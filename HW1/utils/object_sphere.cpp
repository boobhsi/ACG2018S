#include "object_sphere.h"
#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

#define BACK_OCCLUSION false

using namespace std;

Sphere::Sphere(float x, float y, float z, float r) {

#ifdef DEBUG
    cout << "sphere: (" << x << ", " << y << ", " << z << ", " << r << ")\n";
#endif

    centerR.set(x, y, z, r);
}

bool Sphere::checkIntersection(Ray nRay) {
    vec4 d = nRay.getVector();
    vec4 o = nRay.getOrigin();
    vec4 temp = o - centerR;
    vec3 abc(temp[0], temp[1], temp[2]);
    float b = 2 * abc[0] * d[0] + 2 * abc[1] * d[1] + 2 * abc[2] * d[2];
    float a = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
    float c = abc[0] * abc[0] + abc[1] * abc[1] + abc[2] * abc[2] - centerR[3] * centerR[3];
    float judge = pow(b, 2) - 4 * a * c;
    float ansm = (-b - sqrt(judge)) / 2 / a;
    float ansp = (-b + sqrt(judge)) / 2 / a;
    float big_t = max(ansm, ansp);
    float small_t = min(ansm, ansp);
    if(judge >= 0 && big_t > 0.0f) {
        if(small_t < 0.0f) {
            if(BACK_OCCLUSION) {
                //inside the sphere
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return true;
        }
    }
    else {
        return false;
    }
}

Sphere::~Sphere() {

}