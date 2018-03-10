#include "object_triangle.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    origin.set(x1, y1, z1, 1.0f);
    disVector[0].set(x2, y2, z2, 1.0f);
    disVector[0] -= origin;
    disVector[1].set(x3, y3, z3, 1.0f);
    disVector[1] -= origin;

#ifdef DEBUG
    cout << "origin: (" << origin[0] << ", " << origin[1] << ", " << origin[2] << ", " << origin[3] << ")\n";
    cout << "disVector[0]: (" << disVector[0][0] << ", " << disVector[0][1] << ", " << disVector[0][2] << ", " << disVector[0][3] << ")\n";
    cout << "disVector[1]: (" << disVector[1][0] << ", " << disVector[1][1] << ", " << disVector[1][2] << ", " << disVector[1][3] << ")\n";
#endif

}

bool Triangle::checkIntersection(Ray nRay) {
    vec4 zero;
    zero[3] = 1.0f;
    mat4 spec(disVector[0], disVector[1], -nRay.getVector(), zero);
    spec = spec.inverse();
    vec4 multi = nRay.getOrigin() - origin;
    vec4 answer = multi * spec; //[s1, s2, t, a]
    if(answer[0] >=0.0f && answer[1] >= 0.0f && answer[0] + answer[1] <= 1.0f && answer[2] > 0.0f) return true;
    return false;
}

Triangle::~Triangle() {

}