#ifndef MESH_H
#define MESH_H

#include "object.h"
#include "material.h"
#include <vector>
#include "ray.h"

class Mesh {
public:
    Mesh();
    ~Mesh();
    void push_back(Object* object);
    void updateTransform(const mat4& transform_matrix);
    void set_material(const Material& in);
    Intersection_info checkIntersection(Ray& nray);
    float get_density();
private:
    Material mMaterial;
    vector<Object*> object_list;
};

#endif