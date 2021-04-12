/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lambertian.h
 * Author: aleksander
 *
 * Created on January 6, 2021, 9:12 PM
 */

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "rtweekend.h"
#include "material.h"

class lambertian : public material
{
private:
    color albedo;
        
public:
    __device__ lambertian(const color &a) : albedo(a) {}
    
    __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState *local_rand_state) const override
    {
        vec3 scatter_direction = rec.normal + random_unit_vector(local_rand_state);
        
        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
        {
            scatter_direction = rec.normal;
        }
        
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};



#endif /* LAMBERTIAN_H */

