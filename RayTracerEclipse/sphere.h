#pragma once

#include "hitable.h"
#include "material.h"

void get_sphere_uv(const vec3&p, float &u, float &v)
{
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1.0f - (phi + M_PI) / (2.0f * M_PI);
	v = (theta + M_PI/2.0f) / M_PI;
}

class sphere : public hitable
{
public:
	sphere() {}
	sphere(vec3 cen, float r, material*material_ptr) : center(cen), radius(r),m_ptr(material_ptr) {}
	virtual bool hit(const ray &r, float tmin, float tmax, hit_record&rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center;
	float radius;
	material *m_ptr;
};

bool sphere::bounding_box(float t0, float t1, aabb& box) const
{
	box  = aabb(center - vec3(radius,radius,radius), center + vec3(radius,radius,radius));
	return true;
}

bool sphere::hit(const ray &r, float tmin, float tmax, hit_record&rec) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b* b - a * c;
	rec.mat_ptr = m_ptr;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(b* b - a * c)) / a;
		if (temp < tmax && temp > tmin)
		{
			rec.t  = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			get_sphere_uv((rec.p - center)/radius, rec.u, rec.v);
			return true;
		}
		temp = (-b + sqrt(b*b- a * c)) /a;
		if (temp < tmax && temp > tmin)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}

	}
	return false;
}

class moving_sphere : public hitable
{
public:
	moving_sphere() {}
	moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) :
	center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {}
	virtual bool hit(const ray&r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	aabb surrounding_box(aabb box0, aabb box1) const;
	vec3 center(float time) const;
	vec3 center0, center1;
	float time0, time1;
	float radius;
	material *mat_ptr;
};

bool moving_sphere::bounding_box(float t0, float t1, aabb& box) const
{
	aabb  boxt0 = aabb(center(t0) - vec3(radius,radius,radius), center(t0) + vec3(radius,radius,radius));
	aabb  boxt1 = aabb(center(t1) - vec3(radius,radius,radius), center(t1) + vec3(radius,radius,radius));
	box  = surrounding_box(boxt0, boxt1);
	return true;
}

aabb moving_sphere::surrounding_box(aabb box0, aabb box1) const
{
	vec3 small( ffmin(box0.min().x(), box1.min().x()),
				ffmin(box0.min().y(), box1.min().y()),
				ffmin(box0.min().z(), box1.min().z()));
	vec3 big( ffmin(box0.max().x(), box1.max().x()),
				ffmin(box0.max().y(), box1.max().y()),
				ffmin(box0.max().z(), box1.max().z()));

	return aabb(small,big);
}

vec3 moving_sphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record &rec) const
{
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b* b - a * c;
	
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t  = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) /a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}

	}
	return false;
}

