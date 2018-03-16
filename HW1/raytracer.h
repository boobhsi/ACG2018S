#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "utils/algebra3.h"
#include <vector>
#include "utils/object.h"

using namespace std;

class Raytracer
{
public:

    Raytracer();
    void set_eye(vec3 input);
    void set_view_direction(vec3 input);
    void set_fov(float input);
    void set_resolution(int w, int h);
    void create_triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    void create_sphere(float x, float y, float z, float r);
    void start_trace();
    void output_file(char* path);
    ~Raytracer();

private:

    void register_object(Object* object);
    void calculate_transform();
    void execute_eye_transform();
    void create_scene();
    void destroy_scene();

    const float distance = 1.0f;

    vec3 eye_pos;
    vec3 view_direction;
    mat4 transform_matrix;
    float fov;
    int resolution[2];
    vector<Object*> object_list;
    Object* current;
    bool** scene;
};

#endif