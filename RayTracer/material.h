#pragma once

#include "ray.h"
#include "hitable.h"
#include "vec3.h"
#include "texture.h"

vec3 random_in_unit_sphere();

vec3 reflect( const vec3& v, const vec3& n);

bool refract(const vec3& v, const vec3&n, float ni_over_nt, vec3& refracted);

float schlick(float cosine, float ref_idx);

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
	virtual vec3 emitted(float u, float v, const vec3&p) const;
};

class lambertian  : public material
{
public:
	lambertian(texture* a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record &rec, vec3& attentuation, ray& scattered) const
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p, r_in.time());
		attentuation = albedo->value(rec.u,rec.v,rec.p);
		return true;
	}

	texture* albedo;
};

class metal : public material
{
public:
	metal(const vec3& a, float f = 0.0f) : albedo(a)
	{
		if (f < 1.0f)
		{
			fuzz = f;
		}
		else
		{
			fuzz = 1;
		}
	}

	virtual bool scatter(const ray& r_in, const hit_record &rec, vec3& attentuation, ray& scattered) const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attentuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	vec3 albedo;
	float fuzz;
};

class dielectric : public material
{
public:
	dielectric(float ri) : ref_idx(ri) {}

	virtual bool scatter(const ray& r_in, const hit_record &rec, vec3& attentuation, ray& scattered) const
	{
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attentuation = vec3(1.0f,1.0f,1.0f);
		vec3 refracted;
		float reflect_prob;
		float cosine;
		if (dot(r_in.direction(),rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0f/ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		if (refract(r_in.direction(),outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine,ref_idx);
		}
		else
		{
			scattered = ray(rec.p, reflected);
			reflect_prob = 1.0f;
		}

		if (((float)rand() / RAND_MAX) < reflect_prob)
		{
			scattered = ray(rec.p, reflected);
		}
		else
		{
			scattered = ray(rec.p, refracted);
		}

		return true;

	}

	float ref_idx;
};

class diffuse_light : public material
{
public:
	diffuse_light(texture *a) : emit(a) {}
	virtual bool scatter (const ray& r_in, const hit_record &rec, vec3& attenuation, ray& scattered) const { return false; }
	virtual vec3 emitted(float u, float v, const vec3 &p) const
	{
		return emit->value(u,v,p);
	}

	texture *emit;
};

class isotropic : public material
{
public:
	isotropic(texture *a) : albedo(a) {}
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 & attenuation, ray& scattered) const 
	{
		scattered = ray(rec.p, random_in_unit_sphere());
		attenuation = albedo->value(rec.u, rec.v,rec.p);
		return true;
	}
	texture *albedo;
};

