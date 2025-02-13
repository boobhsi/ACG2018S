#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "algebra3.h"
#include "ray.h"

class Object
{
public:
    Object() {}
    virtual bool checkIntersection(Ray nRay) = 0;
    virtual void updateTransform(const mat4& transform_matrix) = 0;
    virtual ~Object() {}
};

#endif