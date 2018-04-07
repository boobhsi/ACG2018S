#ifndef OBJECT_SPHERE_H
#define OBJECT_SPHERE_H

#include <objects/object.h>
#include <utils/algebra3.h>
#include <raytracer/ray.h>

class Sphere : public Object
{
public:
    Sphere(float x, float y, float z, float r);
    Intersection_info checkIntersection(Ray& nray);
    void updateTransform(const mat4& transform_matrix);
    ~Sphere();
private:
    vec4 centerR;
    vec4 centerR_T;
};

#endif