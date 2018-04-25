#ifndef OBJECT_SPHERE_H
#define OBJECT_SPHERE_H

#include <include.h>
#include <objects/object.h>
#include <utils/algebra3.h>
#include <raytracer/ray.h>
#include <utils/intersection_info.h>

class Sphere : public Object
{
public:
    Sphere(float x, float y, float z, float r);
    Intersection_info checkIntersection(Ray& nray);
    void updateTransform(const mat4& transform_matrix);
    float operator[] (int i);
    ~Sphere();
    void build_box();
private:
    vec4 centerR;
    vec4 centerR_T;
};

#endif