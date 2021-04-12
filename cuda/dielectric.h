/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   dielectric.h
 * Author: aleksander
 *
 * Created on January 11, 2021, 5:08 PM
 */

#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "rtweekend.h"
#include "material.h"

class dielectric : public material
{
private:
    float ir;// index of refraction
    
        __device__ float reflectance(float cosine, float ref_idx) const
    {
        // Use Schlick's approximation for reflectance.
        float r0 = (1.0f-ref_idx) / (1.0f+ref_idx);
        r0 = r0*r0;
        return r0 + (1.0f-r0)*powf((1.0f - cosine),5);
    }
        
public:

    
    __device__ dielectric(float index_of_refraction) : ir(index_of_refraction) {}
    
    __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState *local_rand_state) const override
    {
        attenuation = color(1.0f, 1.0f, 1.0f);
        
        float refraction_ratio = rec.front_face ? (1.0f/ir) : ir;
        
        vec3 unit_direction = unit_vector(r_in.direction());
        
        float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
        float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        
        bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
        vec3 direction;
        
        float random_float = curand_uniform(local_rand_state);
        
        float reflectanceNumber = reflectance(cos_theta, refraction_ratio);
        
        if (cannot_refract || reflectanceNumber > random_float)
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }
        
        scattered = ray(rec.p, direction);
        return true;
        
        
    }
};

#endif /* DIELECTRIC_H */

