/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   metal.h
 * Author: aleksander
 *
 * Created on January 6, 2021, 9:43 PM
 */

#ifndef METAL_H
#define METAL_H

#include "rtweekend.h"
#include "material.h"

class metal : public material
{
private:
    color albedo;
    float fuzz;
        
public:
    __device__ metal(const color &a, float f) : albedo(a), fuzz(f < 1.0f ? f : 1.0f) {}
    
    __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState *local_rand_state) const override
    {
         vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(local_rand_state));
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
    }
};

#endif /* METAL_H */

