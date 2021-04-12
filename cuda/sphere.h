/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sphere.h
 * Author: aleksander
 *
 * Created on December 30, 2020, 9:29 AM
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"
#include "vec3.h"

class sphere : public hitable
{
    private:
        point3 center;
        float radius;
        material *mat_ptr;
    
    public:
       __device__ sphere () {}
       __device__ sphere(point3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {}
       __device__ virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;
        
};

__device__ bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center;
    
    float a = r.direction().length_squared();
    float half_b = dot(oc, r.direction());
    float c = oc.length_squared() - radius * radius;
    
    float discriminant = half_b*half_b - a * c;
    
    if (discriminant < 0)
    {
        return false;
    }
    
    float sqrtd = sqrt(discriminant);
    
    //find the nearest root that lies in the acceptable range
    float root = (-half_b - sqrtd) / a;
    
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }
    
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) /  radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    
    return true;
}




#endif /* SPHERE_H */

