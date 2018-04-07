#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <utils/algebra3.h>
#include <vector>
#include <objects/object.h>
#include <objects/mesh.h>
#include <lights/light.h>

using namespace std;

class Raytracer
{
public:

    Raytracer();
    void set_eye(vec3 input);
    void set_view_direction(vec3 input);
    void set_fov(float input);
    void set_resolution(int w, int h);
    void create_mesh();
    void create_sphere(float x, float y, float z, float r);
    void create_triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    void resigter_material(float r, float g, float b, float ka, float kd, float ks, float exp, float refl, float refr, float nr);
    void create_light(float x, float y, float z); 
    void start_trace();
    void output_file(char* path);
    ~Raytracer();

private:

    void attach_object(Object* object);
    void register_mesh();
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
    vector<Mesh*> mesh_list;
    vector<Light*> light_list;
    Mesh* current;
    vec3** scene;
};

#endif