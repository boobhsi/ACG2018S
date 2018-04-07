#include "mesh.h"

Mesh::Mesh() {}

Mesh::~Mesh() {
    for(size_t i = 0; i < object_list.size(); i++) {
        delete object_list[i];
    }
}

void Mesh::push_back(Object* object) {
    object_list.push_back(object);
}

void Mesh::updateTransform(const mat4& transform_matrix) {
    for(size_t i = 0; i < object_list.size(); i++) {
        object_list[i]->updateTransform(transform_matrix);
    }
}

void Mesh::set_material(const Material& in) {
    mMaterial = in;
}

Intersection_info Mesh::checkIntersection(Ray& nray) {
    //find nearest object, return nearest surface
    //TODO
}
    
float Mesh::get_density() {
    return mMaterial.density;
}