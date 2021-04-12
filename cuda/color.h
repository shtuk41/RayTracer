/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   color.h
 * Author: aleksander
 *
 * Created on December 29, 2020, 5:03 PM
 */

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "rtweekend.h"



void write_color(std::ostream &out, color pixel_color)
{
    out << static_cast<int>(255.999 * pixel_color.x());
    out << static_cast<int>(255.999 * pixel_color.y());
    out << static_cast<int>(255.999 * pixel_color.z());
    
}

void write_color(std::ofstream &out, color pixel_color)
{
    float r = pixel_color.x();
    float g = pixel_color.y();
    float b = pixel_color.z();
    
    out << static_cast<int>(256 * clamp(r, 0.0f, 0.999f)) << " "
        << static_cast<int>(256 * clamp(g, 0.0f, 0.999f)) << " "
        << static_cast<int>(256 * clamp(b, 0.0f, 0.999f)) << "\n";
    
}


#endif /* COLOR_H */

