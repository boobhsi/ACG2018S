#ifndef OBJECT_SPHERE_H
#define OBJECT_SPHERE_H

#include "object.h"
#include "algebra3.h"
#include "ray.h"

class Sphere : public Object
{
public:
    Sphere(float x, float y, float z, float r);
    bool checkIntersection(Ray nray);
    void updateTransform(const mat4& transform_matrix);
    ~Sphere();
private:
    vec4 centerR;
    vec4 centerR_T;
};

#endif