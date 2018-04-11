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
    Intersection_info intersect;
#ifdef DEBUG
    cout << spec[0][0] << ", " << spec[0][1] << ", " << spec[0][2] << ", " << spec[0][3] << ")\n";
    cout << spec[1][0] << ", " << spec[1][1] << ", " << spec[1][2] << ", " << spec[1][3] << ")\n";
    cout << spec[2][0] << ", " << spec[2][1] << ", " << spec[2][2] << ", " << spec[2][3] << ")\n";
    cout << spec[3][0] << ", " << spec[3][1] << ", " << spec[3][2] << ", " << spec[3][3] << ")\n";
#endif
    bool inverse_success = mat4::inverse(spec);
    if(!inverse_success) {
        intersect.intersected = false;
        return intersect;
    }
    vec4 multi = nRay.getOrigin() - origin_T;
    //vec4 multi = - origin_T;
#ifdef DEBUG
    cout << spec[0][0] << ", " << spec[0][1] << ", " << spec[0][2] << ", " << spec[0][3] << ")\n";
    cout << spec[1][0] << ", " << spec[1][1] << ", " << spec[1][2] << ", " << spec[1][3] << ")\n";
    cout << spec[2][0] << ", " << spec[2][1] << ", " << spec[2][2] << ", " << spec[2][3] << ")\n";
    cout << spec[3][0] << ", " << spec[3][1] << ", " << spec[3][2] << ", " << spec[3][3] << ")\n";
    cout << "("<< norm_T[0] << ", " << norm_T[1] << ", " << norm_T[2] << ", " << norm_T[3] << ")\n";
#endif
    vec4 answer = multi * spec; //[s1, s2, t, a]
    if(answer[0] >=0.0f && answer[1] >= 0.0f && answer[0] + answer[1] <= 1.0f && answer[2] > 0.0f) {
        if((nRay.getVector() * norm_T) < 0.0) { //the front face
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
            //intersect.surface_type = BACK;
            intersect.surface_type = FRONT; //hw2
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