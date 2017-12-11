#include "stdafx.h"
#include "material.h"

vec3 material::emitted(float u, float v, const vec3&p) const 
{
	return vec3(0,0,0);
}

vec3 random_in_unit_sphere()
{
	vec3 p;

	do
	{
		p = 2.0 * vec3((float)rand() / RAND_MAX,(float)rand() / RAND_MAX,(float)rand() / RAND_MAX) - vec3(1,1,1);
	} while(p.squared_length() >= 1.0);

	return p;
}

vec3 reflect( const vec3& v, const vec3& n)
{
	return v - 2*dot(v,n) * n;
}

bool refract(const vec3& v, const vec3&n, float ni_over_nt, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1.0f - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float schlick(float cosine, float ref_idx)
{
	float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
}