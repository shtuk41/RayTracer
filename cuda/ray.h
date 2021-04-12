/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ray.h
 * Author: aleksander
 *
 * Created on December 29, 2020, 5:26 PM
 */

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
    private:
        point3 orig;
        vec3 dir;
    public:
        
    __device__ ray() {}
    __device__ ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}

    __device__     point3 origin() const  { return orig; }
    __device__     vec3 direction() const { return dir; }

    __device__     point3 at(float t) const {
            return orig + t*dir;
        }
    
};

__device__ float hit_sphere(const point3 &center, float radius, const ray& r)
{
    vec3 oc = r.origin() - center;
    float a = r.direction().length_squared();
    float half_b = dot(oc, r.direction());
    float c = oc.length_squared() - radius * radius;
    float discriminant = half_b * half_b - a * c;
    
    if (discriminant < 0)
    {
        return -1.0f;
    }
    else
    {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

#endif /* RAY_H */

