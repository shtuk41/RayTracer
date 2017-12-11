#pragma once

#include "ray.h"
#include "aabb.h"
#include "texture.h"

class material;

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
	material *mat_ptr;
	float u;
	float v;
};

class hitable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};

class xy_rect : public hitable
{
public:
	xy_rect() {}
	xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *mat) :
	x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}
	virtual bool hit(const ray& r, float t0, float t1, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		box = aabb(vec3(x0,y0,k-0.0001f), vec3(x1, y1, k + 0.0001f));
		return true;
	}
	material *mp;
	float x0, x1, y0, y1, k;
};

class xz_rect : public hitable
{
public:
	xz_rect() {}
	xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material *mat) :
	x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {}
	virtual bool hit(const ray& r, float t0, float t1, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		box = aabb(vec3(x0,k-0.0001f,z0), vec3(x1, k + 0.0001f, z1));
		return true;
	}
	material *mp;
	float x0, x1, z0, z1, k;
};

class yz_rect : public hitable
{
public:
	yz_rect() {}
	yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material *mat) :
	y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {}
	virtual bool hit(const ray& r, float t0, float t1, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		box = aabb(vec3(k-0.0001f,y0,z0), vec3(k + 0.0001f, y1,  z1));
		return true;
	}
	material *mp;
	float y0, y1, z0, z1, k;
};

class flip_normals : public hitable
{
public:
	flip_normals(hitable* p) : ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
	{
		if (ptr->hit(r, t_min, t_max, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual bool bounding_box(float t0, float t1, aabb&box) const
	{
		return ptr->bounding_box(t0,t1,box);
	}

	hitable *ptr;
};

class box : public hitable
{
public:
	box() {}
	box(const vec3 &p0, const vec3& p1, material *ptr);
	virtual bool hit(const ray&r, float t0, float t1, hit_record&rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const 
	{
		box = aabb(pmin, pmax);
		return true;
	}
	vec3 pmin, pmax;
	hitable *list_ptr;
};

class translate : public hitable
{
public:
	translate(hitable *p, const vec3& displacement) : ptr(p), offset(displacement) {}
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb&box) const;
	hitable *ptr;
	vec3 offset;
};

class rotate_y :  public hitable
{
public:
	rotate_y(hitable *p, float angle);
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		box = bbox;
		return hasbox;
	}
	hitable *ptr;
	float sin_theta;
	float cos_theta;
	bool hasbox;
	aabb bbox;
};

class constant_medium : public hitable
{
public:
	constant_medium(hitable *b, float d, texture *a);

	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const
	{
		return boundary->bounding_box(t0,t1, box);
	}
	hitable *boundary;
	float density;
	material *phase_function;
};