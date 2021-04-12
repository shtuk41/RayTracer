//https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/somenewc++features
//https://developer.nvidia.com/blog/accelerated-ray-tracing-cuda/
//http://richiesams.blogspot.com/2015/03/creating-randomness-and-acummulating.html

#include <algorithm>
#include <fstream>
#include <iostream>

#include <cuda.h>
#include <cuda_runtime.h>
#include <curand_kernel.h>
#include <stdio.h>
#include <ctime>

#include <opencv2/opencv.hpp>
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

#define CHECK(call) \
{                                                       \
    const cudaError_t error = call;                     \
    if (error != cudaSuccess)                           \
    {                                                   \
        printf("Error: %s,%d, ", __FILE__, __LINE__);   \
        printf("code: %d, reason: %s\n", error, cudaGetErrorString(error));\
        cudaDeviceReset();\
        exit(1);\
    }\
}

#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line) {
    if (result) {
        std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
        file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}

__device__ color ray_color(const ray &r, hitable **world, curandState *local_rand_state)
{
   ray cur_ray = r;
    vec3 cur_attenuation = vec3(1.0,1.0,1.0);
    for(int i = 0; i < 50; i++) {
        hit_record rec;
        if ((*world)->hit(cur_ray, 0.001f, FLT_MAX, rec)) {
            ray scattered;
            vec3 attenuation;
            if(rec.mat_ptr->scatter(cur_ray, rec, attenuation, scattered, local_rand_state)) {
                cur_attenuation *= attenuation;
                cur_ray = scattered;
            }
            else {
                return vec3(0.0,0.0,0.0);
            }
        }
        else {
            vec3 unit_direction = unit_vector(cur_ray.direction());
            float t = 0.5f*(unit_direction.y() + 1.0f);
            vec3 c = (1.0f-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
            return cur_attenuation * c;
        }
    }
    return vec3(0.0,0.0,0.0); // exceeded recursion
}

__global__ void render_init(int max_x, int max_y, curandState *rand_state)
{
   int i = threadIdx.x + blockIdx.x * blockDim.x;
   int j = threadIdx.y + blockIdx.y * blockDim.y;
   
   if((i >= max_x) || (j >= max_y)) 
       return;
   
   int pixel_index = j*max_x + i;
   //Each thread gets same seed, a different sequence number, no offset
   //curand_init(1984, pixel_index, 0, &rand_state[pixel_index]);
   curand_init(pixel_index, 0, 0, &rand_state[pixel_index]);
}

__global__ void render(vec3 *fb, int max_x, int max_y, int ns, camera **cam, hitable **world, curandState *rand_state) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if((i >= max_x) || (j >= max_y)) return;
    int pixel_index = j*max_x + i;
    
    curandState local_rand_state = rand_state[pixel_index];
    vec3 col(0,0,0);
    
    for (int s = 0; s < ns; s++)
    {
    
        float u = float(i + curand_uniform(&local_rand_state)) / (max_x);
        float v = float(j + curand_uniform(&local_rand_state)) / (max_y);
        ray r = (*cam)->get_ray(u,v);
        col += ray_color(r, world, &local_rand_state);
    }
    
    fb[pixel_index] = (col/float(ns)).squareroot();
}

__global__ void create_world(hitable **d_list, hitable **d_world, camera **cam)
{
    if (threadIdx.x == 0 && blockIdx.x == 0) 
    {
        lambertian *material_ground = new lambertian(color(0.8f, 0.8f, 0.0f));
        lambertian *material_center = new lambertian(color(0.1f, 0.2f, 0.5f));
        
        //metal* material_left   = new metal(color(0.8f, 0.8f, 0.8f), 0.2f);
        
        dielectric* material_left   = new dielectric(1.5f);
        metal* material_right  = new metal(color(0.8f, 0.6f, 0.2f), 0.0f);
        
        *(d_list)   = new sphere(point3( 0.0, -100.5, -1.0), 100.0, material_ground);
        *(d_list+1) = new sphere(point3( 0.0,    0.0, -1.0),   0.5, material_center);
        *(d_list+2) = new sphere(point3( -1.0,    0.0, -1.0),   0.5, material_left);
        *(d_list+3) = new sphere(point3( 1.0,    0.0, -1.0),   0.5, material_right);
        *d_world    = new hitable_list(d_list,4);
        *cam        = new camera();
    }
}

__global__ void free_world(hitable **d_list, hitable **d_world, camera **d_camera) {
    delete *(d_list);
    delete *(d_list+1);
    delete *(d_list+2);
    delete *(d_list+3);
    delete *d_world;
    delete *d_camera;
}

int main(int argc, char **argv)
{  
    //image
    camera **d_camera;
    CHECK(cudaMalloc((void**)&d_camera, sizeof(camera *)));
    
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 2000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    
 
    
    hitable **d_list;
    CHECK(cudaMalloc((void**)&d_list, 2 * sizeof(hitable *)));
    hitable **d_world;
    CHECK(cudaMalloc((void **)&d_world, sizeof(hitable *)));
    create_world<<<1,1>>>(d_list,d_world, d_camera);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());
    
    int nx = image_width;
    int ny = image_height;
    int num_pixels = nx*ny;
    size_t fb_size = num_pixels*sizeof(vec3);

    // allocate FB
    vec3 *fb;
    CHECK(cudaMallocManaged((void **)&fb, fb_size));
     
    int tx = 32;
    int ty = 32;
    
    // Render our buffer
    dim3 blocks(nx/tx+1,ny/ty+1);
    dim3 threads(tx,ty);
    
     curandState *d_rand_state;
    CHECK(cudaMalloc((void **)&d_rand_state, num_pixels * sizeof(curandState)));
    
    render_init<<<blocks, threads>>>(nx, ny, d_rand_state);
    
    int number_samples_per_pixel = 500;
    
    
    render<<<blocks, threads>>>(fb, nx, ny, number_samples_per_pixel, d_camera, d_world, d_rand_state);
    CHECK(cudaGetLastError());
    CHECK(cudaDeviceSynchronize());
    
    // Output FB as Image
    std::ofstream imageFile;
    
    imageFile.open("imageFile.ppm");
    
    if (!imageFile.is_open())
    {
        std::cout << "unable to open file" << std::endl;
        return -1;
    }
    
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--) 
    {
        for (int i = 0; i < nx; i++) 
        {
       
            size_t pixel_index = j*nx + i;
                        
            write_color(imageFile, fb[pixel_index]);
            
            //imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }
    
    imageFile.close();
    
    checkCudaErrors(cudaDeviceSynchronize());
    free_world<<<1,1>>>(d_list,d_world,d_camera);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaFree(d_list));
    checkCudaErrors(cudaFree(d_world));
    checkCudaErrors(cudaFree(d_camera));
    checkCudaErrors(cudaFree(fb));
    
     // useful for cuda-memcheck --leak-check full
    cudaDeviceReset();
    
    return 0;

}




        
