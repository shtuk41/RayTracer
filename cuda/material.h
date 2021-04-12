/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   material.h
 * Author: aleksander
 *
 * Created on January 6, 2021, 8:59 PM
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "vec3.h"

struct hit_record;

class material
{
public:
    __device__ virtual bool scatter(const ray & r_in, const hit_record &rec, color &attenuation, ray& scattered, curandState *local_rand_state) const = 0;
};



#endif /* MATERIAL_H */

