#ifndef RAY_H
#define RAY_H

#include "algebra3.h"

class Ray
{
public:
    Ray(const vec3& to);
    static void setOrigin(vec3& input);
    static vec4 getOrigin();
    vec4 getVector();
private:
    static vec4 origin;
    vec4 disVector;
};

#endif