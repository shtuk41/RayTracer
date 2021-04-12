// RayTracer.cpp : Defines the entry point for the console application.
//

#include <float.h>
#include <fstream>
#include <time.h>

#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

vec3 color(const ray&r, hitable *world, int depth)
{
	hit_record rec;

	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0,0,0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f,1.0f,1.0f) + t * vec3(0.5f,0.7f, 1.0f);
	}
}

hitable* random_scene()
{
	int n = 50000;
	hitable **list = new hitable*[n+1];
	texture *checker = new checker_texture(new constant_texture(vec3(0.2f,0.3f,0.1f)),new constant_texture(vec3(0.9f,0.9f,0.9f)));
	list[0] = new sphere(vec3(0,-1000,0),1000, new lambertian(checker));
	int i = 1;

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = get_random();
			vec3 center(a + 0.9f * get_random(), 0.2f,b + 0.9f * get_random());
			if ((center - vec3(4,0.2f,0)).length() > 0.9f)
			{
				if (choose_mat < 0.5f)
				{
					list[i++] = new moving_sphere(center, center + vec3(0,0.5f * get_random(),0),0.0f, 1.0f,0.2f, new lambertian(new constant_texture(vec3(get_random() * get_random(), get_random() * get_random(), get_random()))));
				}
				else if (choose_mat < 0.8f)
				{
					list[i++] = new sphere(center,0.2f, new metal(vec3(0.5f*(1.0f + get_random()),0.5f*(1.0f + get_random()),0.5f*(1.0f + get_random())),0.5f*get_random()));
				}
				else
				{
					list[i++] = new sphere(center,0.2f, new dielectric(1.5f));
				}
			}
		}
	}


	list[i++] = new sphere(vec3(0,1,0),1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4,1,0),1.0, new lambertian(new constant_texture(vec3(0.4f,0.2f,0.1f))));
	list[i++] = new sphere(vec3(4,1,0),1.0f, new metal(vec3(0.7f,0.6f,0.5f),0.0));

	return new hitable_list(list,i);
}


int main(int argc, char* argv[])
{

	srand((unsigned int)time(NULL));

	time_t start = clock();
	int nx = 640;
	int ny = 480;
	int ns = 100;
	
	std::ofstream frame;
	frame.open("frame_b2_chapter3_640_480.ppm", std::ios::trunc);

	if (frame.is_open())
	{
		frame << "P3\n" << nx << " " << ny << "\n255\n";
		hitable *list[5];
		list[0] = new sphere(vec3(0,0,-1),0.5f, new lambertian(new constant_texture(vec3(0.1f,0.2f,0.5f))));
		list[1] = new sphere(vec3(0,-100.5,-1),100, new lambertian(new constant_texture(vec3(0.8f,0.8f,0.0f))));
		list[2] = new sphere(vec3(1,0,-1),0.5f, new metal(vec3(0.8f,0.6f,0.2f),0.0f));
		list[3] = new sphere(vec3(-1,0,-1),0.5f, new dielectric(1.5f));
		list[4] = new sphere(vec3(-1,0,-1),-0.45f, new dielectric(1.5f));
	
		//hitable * world = new hitable_list(list,5);

		hitable * world = random_scene();

		vec3 lookfrom(0,1,5);
		vec3 lookat(0,0,-1.0f);
		float dist_to_focus = (lookfrom - lookat).length();
		float aperature = 0.001f;

		camera cam(lookfrom, lookat, vec3(0,1,0), 90, float(nx)/float(ny),aperature,dist_to_focus, 0.0f, 1.0f);

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

