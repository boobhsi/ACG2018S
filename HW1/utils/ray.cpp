#include "ray.h"

/*
vec4 Ray::origin;

void Ray::setOrigin(vec3& input) {
    Ray::origin[0] = input[0];
    Ray::origin[1] = input[1];
    Ray::origin[2] = input[2];
    Ray::origin[3] = 1.0f;
}

vec4 Ray::getOrigin() {
    return Ray::origin;
}
*/

Ray::Ray(const vec3& to) {
    vec4 temp(to, 0.0f);
    //disVector = temp - origin;
    disVector = temp;
}

vec4 Ray::getVector() {
    return disVector;
}