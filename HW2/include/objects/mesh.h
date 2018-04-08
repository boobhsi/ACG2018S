#ifndef MESH_H
#define MESH_H

#include <objects/object.h>
#include <materials/material.h>
#include <vector>
#include <raytracer/ray.h>

class Mesh 
{
public:
    Mesh();
    ~Mesh();
    void push_back(Object* object);
    void updateTransform(const mat4& transform_matrix);
    void set_material(const Material& in);
    Intersection_info checkIntersection(Ray& nray);
    float get_density();
    vec3 get_color();
    vec3 get_properties();
    float get_specular();
    vec2 get_scatter_ratio();
private:
    Material mMaterial;
    vector<Object*> object_list;
    bool rigid;
};

#endif