/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   hitable_list.h
 * Author: aleksander
 *
 * Created on December 30, 2020, 10:09 AM
 */

#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.h"

class hitable_list : public hitable
{
    private:
        hitable **list;
        int list_size;
        
    public:
        __device__ hitable_list() {}
        __device__ hitable_list(hitable **l, int n) 
        {
            list = l; 
            list_size = n;
        }
        __device__ virtual bool hit(const ray&r, float t_min, float t_max, hit_record&rec) const;
        
};

__device__ bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const 
{
        hit_record temp_rec;
        bool hit_anything = false;
        float closest_so_far = t_max;
        for (int i = 0; i < list_size; i++) {
            if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
}

#endif /* HITABLE_LIST_H */

