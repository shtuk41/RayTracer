#include <limits>
#include "hitable.h"
#include "hitable_list.h"
#include "material.h"

#define FLT_MAX std::numeric_limits<float>::max()

bool xy_rect::hit(const ray&r, float t0, float t1, hit_record &rec) const
{
	float t = (k-r.origin().z()) /r.direction().z();
	if (t < t0 || t > t1)
		return false;
	float x = r.origin().x() + t* r.direction().x();
	float y = r.origin().y() + t* r.direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;
	rec.u = (x-x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0,0,1);
	return true;
}

bool xz_rect::hit(const ray&r, float t0, float t1, hit_record &rec) const
{
	float t = (k-r.origin().y()) /r.direction().y();
	if (t < t0 || t > t1)
		return false;
	float x = r.origin().x() + t* r.direction().x();
	float z = r.origin().z() + t* r.direction().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;
	rec.u = (x-x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0,1,0);
	return true;
}

bool yz_rect::hit(const ray&r, float t0, float t1, hit_record &rec) const
{
	float t = (k-r.origin().x()) / r.direction().x();
	if (t < t0 || t > t1)
		return false;
	float y = r.origin().y() + t* r.direction().y();
	float z = r.origin().z() + t* r.direction().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;
	rec.u = (y-y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(1,0,0);
	return true;
}

box::box(const vec3 &p0, const vec3 &p1, material *ptr)
{
	pmin = p0;
	pmax = p1;
	hitable **list = new hitable*[6];
	list[0] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(),  p1.z(), ptr);
	list[1] = new flip_normals(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(),  p0.z(), ptr));
	list[2] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(),  p1.y(), ptr);
	list[3] = new flip_normals(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(),  p0.y(), ptr));
	list[4] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(),  p1.x(), ptr);
	list[5] = new flip_normals(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(),  p1.x(), ptr));
	list_ptr = new hitable_list(list,6);

}

bool box::hit(const ray & r, float t0, float t1, hit_record &rec) const
{
	return list_ptr->hit(r, t0, t1, rec);
}

bool translate::hit(const ray& r, float t_min, float t_max, hit_record &rec) const
{
	ray moved_r(r.origin() - offset, r.direction(), r.time());
	if (ptr->hit(moved_r, t_min, t_max, rec))
	{
		rec.p += offset;
		return true;
	}
	else
		return false;
}

bool translate::bounding_box(float t0, float t1, aabb& box) const
{
	if (ptr->bounding_box(t0, t1, box))
	{
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}
	else
	{
		return false;
	}
}

rotate_y::rotate_y(hitable *p, float angle) : ptr(p)
{
	float radians = (M_PI / 180.0f) * angle;
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0,1, bbox);
	vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max(-FLT_MAX,-FLT_MAX,-FLT_MAX);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
				float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
				float z = k * bbox.max().z() + (1 - k) * bbox.min().z();
				float newx = cos_theta * x + sin_theta*z;
				float newz = -sin_theta*x + cos_theta*z;
				vec3 tester(newx,y, newz);
				for (int c= 0; c < 3; c++)
				{
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}

	bbox = aabb(min, max);
}

bool rotate_y::hit(const ray&r, float t_min, float t_max, hit_record&rec) const
{
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
	origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
	direction[0] = cos_theta*r.direction()[0] - sin_theta * r.direction()[2];
	direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];
	ray rotated_r(origin,direction,r.time());
	if (ptr->hit(rotated_r,t_min,t_max,rec))
	{
		vec3 p = rec.p;
		vec3 normal = rec.normal;
		p[0] = cos_theta * rec.p[0] + sin_theta*rec.p[2];
		p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];
		normal[0] = cos_theta * rec.normal[0] + sin_theta*rec.normal[2];
		normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	else
	{
		return false;
	}
}

constant_medium::constant_medium(hitable *b, float d, texture *a) : boundary(b), density(d)
{
	phase_function = new isotropic(a);
}

bool constant_medium::hit(const ray& r, float t_min, float t_max, hit_record & rec) const
{
	bool db = (get_random() < 0.00001);
	db = false;
	hit_record rec1, rec2;
	if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1))
	{
		if (boundary->hit(r,rec1.t + 0.0001f, FLT_MAX, rec2))
		{
			if (db) 
			{
				std::cout << "\nt0 t1 " << rec1.t << " " << rec2.t << std::endl;
			}
			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0)
				rec1.t = 0;
			float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
			float hit_distance = -(1/density) * log(get_random());
			if (hit_distance < distance_inside_boundary)
			{
				if (db)
					std::cout << "hit_distance = " << hit_distance << std::endl;
				rec.t = rec1.t + hit_distance / r.direction().length();
				if (db)
					std::cout << "rec.t = " << rec.t << std::endl;
				rec.p = r.point_at_parameter(rec.t);
				if (db)
					std::cout << "rec.p  = " << rec.p << std::endl;
				rec.normal - vec3(1,0,0);//arbitrary
				rec.mat_ptr = phase_function;
				return true;
			}
		}
	}

	return false;
}

