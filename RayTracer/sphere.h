#pragma once

#include "hitable.h"
#include "material.h"

class sphere : public hitable
{
public:
	sphere() {}
	sphere(vec3 cen, float r, material*material_ptr) : center(cen), radius(r),m_ptr(material_ptr) {}
	virtual bool hit(const ray &r, float tmin, float tmax, hit_record&rec) const;
	vec3 center;
	float radius;
	material *m_ptr;
};

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