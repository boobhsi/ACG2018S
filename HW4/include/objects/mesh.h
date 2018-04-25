#ifndef MESH_H
#define MESH_H

#include <include.h>
#include <objects/object.h>
#include <materials/material.h>
#include <vector>

using namespace std;

class Mesh 
{
public:
    Mesh();
    ~Mesh();
    void set_material(const Material& in);
    void push_back(Object* object);
    float get_density();
    vec3 get_color();
    vec3 get_properties();
    float get_specular();
    vec2 get_scatter_ratio();
    //KDTree* get_tree();
private:
    Material mMaterial;
    bool rigid;
    vector<Object*> object_list;
    //KDTree* tree;
};

#endif