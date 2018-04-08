#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <utils/algebra3.h>
#include <raytracer/ray.h>

typedef enum { FRONT, BACK, RIGID } Surface_type;

struct Intersection_info 
{
    bool intersected;
    vec4 intersect_point;
    vec4 norm;
    float t;
    Surface_type surface_type;
};

class Object
{
public:
    Object() {}
    virtual Intersection_info checkIntersection(Ray& nRay) = 0;
    virtual void updateTransform(const mat4& transform_matrix) = 0;
    virtual ~Object() {}
private:
};

#endif