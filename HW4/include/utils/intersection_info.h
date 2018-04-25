#ifndef INTERSECTION_INFO_H
#define INTERSECTION_INFO_H

#include <include.h>
#include <utils/algebra3.h>
#include <objects/object.h>

typedef enum { FRONT, BACK, RIGID } Surface_type;

typedef struct 
{
    bool intersected = false;
    vec4 intersect_point;
    vec4 norm;
    float t = -1.0f;
    Surface_type surface_type = FRONT;
    Object* surface;
} Intersection_info;

#endif