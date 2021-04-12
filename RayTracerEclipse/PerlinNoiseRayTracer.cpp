// RayTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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

vec3 color(const ray&r, hitable *world, int depth)
{
	hit_record rec;

	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted =  rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

		if (depth < 1 && rec.mat_ptr->scatter(r,rec,attenuation,scattered))
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

hitable* two_perlin_spheres()
{
	texture * pertext = new noise_texture(10);
	hitable **list = new hitable *[2];
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("anton.jpg",&nx,&ny,&nn,0);
	material *mat = new lambertian(new image_texture(tex_data,nx,ny));

	list[0] = new sphere(vec3(0,-1000,0),1000, new lambertian( pertext));
	list[1] = new sphere(vec3(0,2,0),2, mat);
	return new hitable_list(list,2);
}

hitable * simple_light()
{
	texture *pertext = new noise_texture(4);
	hitable **list = new hitable*[4];
	list[0] = new sphere(vec3(0,-1000,0),1000, new lambertian( pertext));
	list[1] = new sphere(vec3(0,2,0),2, new lambertian( pertext));
	list[2] = new sphere(vec3(0,7,0),2, new diffuse_light( new constant_texture(vec3(4,4,4))));
	list[3] = new xy_rect(3,5,1,3,-2,new diffuse_light( new constant_texture(vec3(4,4,4))));

	return new hitable_list(list,4);
}


int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned int)time(NULL));

	time_t start = clock();
	int nx = 640;
	int ny = 480;
	int ns = 100;
	
	std::ofstream frame;
	frame.open("frame_b2_chapter6_640_480.ppm", std::ios::trunc);

	if (frame.is_open())
	{
		frame << "P3\n" << nx << " " << ny << "\n255\n";
	
		//hitable * world = new hitable_list(list,5);

		//hitable * world = two_perlin_spheres();
		hitable * world = simple_light();

		vec3 lookfrom(25,2,0);
		vec3 lookat(0,0,0);
		float dist_to_focus = 25;
		float aperature = 0;

		camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny),aperature,dist_to_focus, 0.0f, 1.0f);

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

	system("pause");

	return 0;
}

