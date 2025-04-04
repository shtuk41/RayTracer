#pragma once

#include "ray.h"
#include "random.h"

vec3 random_in_unit_disk()
{
	vec3 p;

	do
	{
		p = 2.0f * vec3(get_random(), get_random(), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
	}	while (dot(p,p) >= 1.0f);
	return p;
}

class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperature, float focus_dist, float t0, float t1) //vfov is top to bottom in degrees
	{
		time0 = t0;
		time1 = t1;
		lens_radius = aperature / 2.0f;
		float theta = vfov * M_PI/180.0f;
		float half_height = tan(theta/2.0f);
		float half_width = aspect * half_height;

		origin = lookfrom;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup,w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * focus_dist * u - half_height *focus_dist* v - focus_dist *w;
		horizontal = 2.0f * half_width * focus_dist * u;
		vertical = 2.0f * half_height * focus_dist * v;
	}

	ray get_ray(float s, float t)
	{
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		float time = time0 + get_random() * (time1 - time0);
		return ray(origin + offset, lower_left_corner + s*horizontal + t * vertical - origin - offset, time); 
	}

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lens_radius;
	float time0, time1;
};
