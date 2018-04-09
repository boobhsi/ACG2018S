#ifndef RAY_H
#define RAY_H

#include <utils/algebra3.h>
#include <objects/mesh.h>
#include <vector>
#include <raytracer/raytracer.h>

class Raytracer;

#define RECURSIVE_LIMIT 0
typedef vec3 (Raytracer::*SHADING)(vec4, vec4, vec4, vec3 , vec3 , float, int);

using namespace std;

class Ray
{
public:
    Ray(const vec3& from, const vec3& to_dir, int num);
    vec4 getOrigin();
    vec4 getVector();
    vec3 trace_the_world(const vector<Mesh*>& object_list);
    vec3 trace_the_inside(Mesh* object_inside, const vector<Mesh*>& object_list);
    int check_nearest(const vector<Mesh*>& object_list, vec4 point);
private:
    vec4 origin;
    vec4 disVector;
    int recursive_num;
    Ray get_refraction(vec4 point, float d_in, float d_out, vec4 norm);
    Ray get_reflection(vec4 point, vec4 norm);
    int get_recursive_num();
};

#endif