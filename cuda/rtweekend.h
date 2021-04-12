/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   rtweekend.h
 * Author: aleksander
 *
 * Created on December 30, 2020, 10:24 AM
 */

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include "ray.h"

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float degrees_to_radians(float degrees)
{
    return degrees * pi / 180.0f;
}

inline double clamp(float x, float min, float max)
{
    if (x < min)
    {
        return min;
    }
    
    if (x > max)
    {
        return max;
    }
    
    return x;
}




#endif /* RTWEEKEND_H */

