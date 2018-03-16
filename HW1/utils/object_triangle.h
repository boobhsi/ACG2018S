#ifndef OBJECT_TRIANGLE_H
#define OBJECT_TRIANGLE_H

#include "object.h"
#include "algebra3.h"
#include "ray.h"

class Triangle : public Object
{
public:
    Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    bool checkIntersection(Ray nRay);
    void updateTransform(const mat4& transform_matrix);
    ~Triangle();
private:
    vec4 origin;
    vec4 origin_T;
    vec4 disVector[2];
    vec4 disVector_T[2];
};

#endif