// RayTracer.cpp : Defines the entry point for the console application.
//

#include <float.h>
#include <fstream>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"
#include "perlin.h"
#include "bvh_node.h"

vec3 color(const ray&r, hitable *world, int depth)
{
	hit_record rec;

	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted =  rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

		if (depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		//return (1.0f - t) * vec3(1.0f,1.0f,1.0f) + t * vec3(0.5f,0.7f, 1.0f);

		return vec3(0,0,0);
	}
}

hitable *final()
{
	int nb = 20;
	hitable **list = new hitable*[30];
	hitable **boxlist = new hitable*[10000];
	hitable **boxlist2 = new hitable*[10000];
	material *white = new lambertian(new constant_texture(vec3(0.73f,0.73f,0.73f)));
	material *ground = new lambertian(new constant_texture(vec3(0.48f,0.83f,0.53f)));
	int b = 0;
	for (int i = 0; i < nb; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			float w = 100;
			float x0 = -1000 + i*w;
			float z0 = -1000 + j* w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (get_random() + 0.01f);
			float z1 = z0 + w;
			boxlist[b++] = new box(vec3(x0,y0,z0),vec3(x1,y1,z1), ground);
		}
	}
	int l = 0;
	list[l++] = new bvh_node(boxlist,b,0,1);
	material * light = new diffuse_light( new constant_texture(vec3(15,15,15)));
	list[l++] = new xz_rect(123,423,147,412,554,light);
	vec3 center(400,400,200);
	list[l++] = new moving_sphere(center, center + vec3(30,0,0),0,1,50,new lambertian(new constant_texture(vec3(0.7f,0.3f,0.1f))));
	list[l++] = new sphere(vec3(260,150,45),50, new dielectric(1.5));
	list[l++] = new sphere(vec3(0,150,145),50, new metal(vec3(0.8f,0.8f,0.9f),10.0f));
	hitable *boundary = new sphere(vec3(360,150,145),70, new dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary,0.2f, new constant_texture(vec3(0.2f,0.4f,0.9f)));
	boundary = new sphere(vec3(0,0,0),5000,new dielectric(1.5));
	list[l++] = new constant_medium(boundary,0.0001f, new constant_texture(vec3(1.0f,1.0f,1.0f)));
	int nx,ny, nn;
	unsigned char *text_data = stbi_load("anton.jpg", &nx,&ny,&nn,0);
	material *emat = new lambertian(new image_texture(text_data,nx,ny));
	list[l++] = new sphere(vec3(400,200,400),100,emat);
	texture *pertext = new noise_texture(0.1f);
	list[l++] = new sphere(vec3(220,280,300),80, new lambertian(pertext));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxlist2[j] = new sphere(vec3(165*get_random(), 165 * get_random(), 165 * get_random()), 10, white);
	}

	list[l++] = new translate(new rotate_y(new bvh_node(boxlist2,ns,0.0,1.0), 15), vec3(-100,270,395));



	return new hitable_list(list,l);



}

int main()
{
	srand((unsigned int)time(NULL));

	time_t start = clock();
	int nx = 640;
	int ny = 480;
	int ns = 1000;
	
	std::ofstream frame;
	frame.open("frame_b2_final_2_640_480.ppm", std::ios::trunc);

	if (frame.is_open())
	{
		frame << "P3\n" << nx << " " << ny << "\n255\n";
	
		//hitable * world = new hitable_list(list,5);

		//hitable * world = two_perlin_spheres();
		//hitable * world = cornell_box();
		//hitable * world = cornell_smoke();

		hitable * world = final();

		vec3 lookfrom(278,278,-800);
		vec3 lookat(278,278,0);
		float dist_to_focus = 10.0f;
		float aperature = 0.0f;
		float vfov = 40.0f;

		camera cam(lookfrom, lookat, vec3(0,1,0), vfov, float(nx)/float(ny), aperature, dist_to_focus, 0.0f,1.0f);

		time_t start_row = clock();

		for (int j = ny-1;j >= 0; j--)
		{
			for (int i = 0; i < nx; i++)
			{
				vec3 col(0,0,0);
				for (int s = 0;s < ns; s++)
				{
					float u = float(i + get_random())/float(nx);
					float v = float(j + get_random())/float(ny);
					ray r = cam.get_ray(u,v);
					//vec3 = r.point_at_parameter(2.0);
					col += color(r, world, 0);
				}
				col /= float(ns);
				col = vec3(sqrt(col[0]), sqrt(col[1]),sqrt(col[2]));
				int ir = int(255.99 * col[0]);
				int ig = int(255.99 * col[1]);
				int ib = int(255.99 * col[2]);
				frame << ir << " " << ig << " " << ib << "\n";
			}

			time_t t = clock() - start_row;
			std::cout << "Processing Y: " << j << " took " << ((float)t / CLOCKS_PER_SEC) << std::endl;
			start_row = clock();
		}

		delete world;

	}

	std::cout << "Total time: " << (float)(clock() - start) / CLOCKS_PER_SEC << std::endl;

	return 0;
}

