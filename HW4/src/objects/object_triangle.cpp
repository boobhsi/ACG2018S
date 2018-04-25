#include <objects/object_triangle.h>
#include <raytracer/raytracer.h>
#include <cmath>

using namespace std;

#ifdef DEBUG
#include <iostream>
#endif

#define BOX_OFFSET 0.0001f

extern Raytracer* rt;

Triangle::Triangle() {

}

Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    origin.set(x1, y1, z1, 1.0f);
    disVector[0].set(x2, y2, z2, 1.0f);
    disVector[0] -= origin;
    disVector[1].set(x3, y3, z3, 1.0f);
    disVector[1] -= origin;
    //center.set((x1 + x2 + x3) / 3.0f, (y1 + y2 + y3) / 3.0f, (z1 + z2 + z3) / 3.0f, 1.0f);

    vec3 temp1(disVector[0][0], disVector[0][1], disVector[0][2]), temp2(disVector[1][0], disVector[1][1], disVector[1][2]);
    vec3 temp3;
    temp3 = (temp1 ^ temp2).normalize();

    norm.set(temp3[0], temp3[1], temp3[2], 0.0f);
}

Triangle::Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float n1, float n2, float n3) {
    origin.set(x1, y1, z1, 1.0f);
    disVector[0].set(x2, y2, z2, 1.0f);
    disVector[0] -= origin;
    disVector[1].set(x3, y3, z3, 1.0f);
    disVector[1] -= origin;
    //center.set((x1 + x2 + x3) / 3.0f, (y1 + y2 + y3) / 3.0f, (z1 + z2 + z3) / 3.0f, 1.0f);
    norm.set(n1, n2, n3, 0.0f);
}

Intersection_info Triangle::checkIntersection(Ray& nRay) {

    rt->cal_once();

    vec4 zero;
    zero[3] = 1.0f;
    mat4 spec(disVector_T[0], disVector_T[1], -nRay.getVector(), zero);
    Intersection_info intersect;

    bool inverse_success = mat4::inverse(spec);
    if(!inverse_success) {
        return intersect;
    }
    vec4 multi = nRay.getOrigin() - origin_T;

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
            intersect.norm = norm_T;
            intersect.t = answer[2];
            intersect.intersect_point = origin_T + answer[0] * disVector_T[0] + answer[1] * disVector_T[1];
            intersect.surface_type = BACK;
        }
    }
    intersect.surface = this;
    return intersect;
}

void Triangle::updateTransform(const mat4& transform_matrix) {

    mat4 temp = transform_matrix;

    origin_T = origin * temp;
    disVector_T[0] = disVector[0] * temp;
    disVector_T[1] = disVector[1] * temp;
    norm_T = norm * temp;
    //center_T = center * temp;
}

Triangle::~Triangle() {

}

void Triangle::set_points(vec4 p1, vec4 p2, vec4 p3) {
    origin = p1;
    disVector[0] = p2;
    disVector[0] -= origin;
    disVector[1] = p3;
    disVector[1] -= origin;

    vec3 temp1(disVector[0][0], disVector[0][1], disVector[0][2]), temp2(disVector[1][0], disVector[1][1], disVector[1][2]);
    vec3 temp3;
    temp3 = (temp1 ^ temp2).normalize();

    norm.set(temp3[0], temp3[1], temp3[2], 0.0f);

    //center = ( p1 + p2 + p3 ) / 3.0f;

    origin_T = origin;
    disVector_T[0] = disVector[0];
    disVector_T[1] = disVector[1];
    norm_T = norm;
    //center_T = center;
}

float Triangle::operator[] (int i) {
    return center_T[i];
}

void Triangle::build_box() {
    vec4 temp1 = origin_T + disVector_T[0];
    vec4 temp2 = origin_T + disVector_T[1];
    mbx.m_x = max(origin_T[0], max(temp1[0], temp2[0])) + BOX_OFFSET;
    mbx.s_x = min(origin_T[0], min(temp1[0], temp2[0])) - BOX_OFFSET;
    mbx.m_y = max(origin_T[1], max(temp1[1], temp2[1])) + BOX_OFFSET;
    mbx.s_y = min(origin_T[1], min(temp1[1], temp2[1])) - BOX_OFFSET;
    mbx.m_z = max(origin_T[2], max(temp1[2], temp2[2])) + BOX_OFFSET;
    mbx.s_z = min(origin_T[2], min(temp1[2], temp2[2])) - BOX_OFFSET;

    center_T.set((mbx.m_x - mbx.s_x) / 2.0f + mbx.s_x, (mbx.m_y - mbx.s_y) / 2.0f + mbx.s_y, (mbx.m_z - mbx.s_z) / 2.0f + mbx.s_z, 1.0f);

    /*
    mbx.m_x = mbx.s_x = origin_T[0];
    mbx.m_y = mbx.s_y = origin_T[1];
    mbx.m_z = mbx.s_z = origin_T[2];
    if(abs(disVector_T[0][0] - disVector_T[1][0]) < 0.01f) {
        mbx.m_x += 0.005f;
        mbx.s_x -= 0.005f;
    }
    else {
        float m_xd = max(disVector_T[0][0], disVector_T[1][0]);
        float s_xd = min(disVector_T[0][0], disVector_T[1][0]);
        if(m_xd > 0) {
            mbx.m_x += m_xd;
        }
        if(s_xd < 0) {
            mbx.s_x += s_xd;
        }
    }
    if(abs(disVector_T[0][1] - disVector_T[1][1]) < 0.01f) {
        mbx.m_y += 0.005f;
        mbx.s_y -= 0.005f;
    }
    else {
        float m_yd = max(disVector_T[0][1], disVector_T[1][1]);
        float s_yd = min(disVector_T[0][1], disVector_T[1][1]);
        if(m_yd > 0) {
            mbx.m_y += m_yd;
        }
        if(s_yd < 0) {
            mbx.s_y += s_yd;
        }
    }
    if(abs(disVector_T[0][2] - disVector_T[1][2]) < 0.01f) {
        mbx.m_z += 0.005f;
        mbx.s_z -= 0.005f;
    }
    else {
        float m_zd = max(disVector_T[0][2], disVector_T[1][2]);
        float s_zd = min(disVector_T[0][2], disVector_T[1][2]);
        if(m_zd > 0) {
            mbx.m_z += m_zd;
        }
        if(s_zd < 0) {
            mbx.s_z += s_zd;
        }
    }
    */
}