/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   hittable.h
 * Author: aleksander
 *
 * Created on December 30, 2020, 9:27 AM
 */

#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class material;


struct hit_record
{
    point3 p;
    vec3 normal;
    float t;
    bool front_face;
    material * mat_ptr;
    
    __device__ void set_face_normal(const ray &r, const vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hitable
{
public:
    __device__ virtual bool hit(const ray &r, float t_min, float t_max, hit_record& rec) const = 0;
};


#endif /* HITTABLE_H */

