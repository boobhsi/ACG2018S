#include <objects/object_triangle.h>

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

Intersection_info Triangle::checkIntersection(Ray& nRay) {
    vec4 zero;
    zero[3] = 1.0f;
    mat4 spec(disVector_T[0], disVector_T[1], -nRay.getVector(), zero);
    spec = spec.inverse();
    vec4 multi = nRay.getOrigin() - origin_T;
    //vec4 multi = - origin_T;
    vec4 answer = multi * spec; //[s1, s2, t, a]
    Intersection_info intersect;
    if(answer[0] >=0.0f && answer[1] >= 0.0f && answer[0] + answer[1] <= 1.0f && answer[2] > 0.0f) {
        if(nRay.getVector().normalize() * norm_T > 0.0) { //the front face
            intersect.intersected = true;
            intersect.norm = norm_T;
            intersect.t = answer[2];
            intersect.intersect_point = origin_T + answer[0] * disVector_T[0] + answer[1] * disVector_T[1];
            intersect.surface_type = FRONT;
        }
        else {
            intersect.intersected = true;
            intersect.norm = -norm_T;
            intersect.t = answer[2];
            intersect.intersect_point = origin_T + answer[0] * disVector_T[0] + answer[1] * disVector_T[1];
            intersect.surface_type = BACK;
        }
    }
    else {
        intersect.intersected = false;
    }
    return intersect;
}

void Triangle::updateTransform(const mat4& transform_matrix) {

#ifdef DEBUG
    cout << "origin: (" << origin[0] << ", " << origin[1] << ", " << origin[2] << ", " << origin[3] << ")\n";
    cout << "disVector[0]: (" << disVector[0][0] << ", " << disVector[0][1] << ", " << disVector[0][2] << ", " << disVector[0][3] << ")\n";
    cout << "disVector[1]: (" << disVector[1][0] << ", " << disVector[1][1] << ", " << disVector[1][2] << ", " << disVector[1][3] << ")\n";
#endif

    mat4 temp = transform_matrix;

    origin_T = origin * temp;
    disVector_T[0] = disVector[0] * temp;
    disVector_T[1] = disVector[1] * temp;

    vec3 temp1, temp2, temp_norm;
    temp1.set(disVector_T[0][0], disVector_T[0][1], disVector_T[0][2]);
    temp2.set(disVector_T[1][0], disVector_T[1][1], disVector_T[1][2]);
    temp_norm = temp1 ^ temp2;
    temp_norm = temp_norm.normalize();
    norm_T.set(temp_norm[0], temp_norm[1], temp_norm[2], 0.0f);

#ifdef DEBUG
    cout << "origin_T: (" << origin_T[0] << ", " << origin_T[1] << ", " << origin_T[2] << ", " << origin_T[3] << ")\n";
    cout << "disVector_T[0]: (" << disVector_T[0][0] << ", " << disVector_T[0][1] << ", " << disVector_T[0][2] << ", " << disVector_T[0][3] << ")\n";
    cout << "disVector_T[1]: (" << disVector_T[1][0] << ", " << disVector_T[1][1] << ", " << disVector_T[1][2] << ", " << disVector_T[1][3] << ")\n";
#endif
}

Triangle::~Triangle() {

}