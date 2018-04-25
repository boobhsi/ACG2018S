#include <raytracer/ray.h>
#include <objects/mesh.h>
#include <cassert>

using namespace std;

Mesh::Mesh() {
    rigid = false;
}

Mesh::~Mesh() {

}

void Mesh::set_material(const Material& in) {
    mMaterial = in;
    if(mMaterial.density == 0) {
        rigid = true;
    }
}

void Mesh::push_back(Object* object) {
    object_list.push_back(object);
}
    
float Mesh::get_density() {
    return mMaterial.density;
}

vec3 Mesh::get_color() {
    return mMaterial.color;
}

vec3 Mesh::get_properties() {
    return mMaterial.surface_properties;
};

float Mesh::get_specular() {
    return mMaterial.specular;
}

vec2 Mesh::get_scatter_ratio() {
    return mMaterial.scatter_ratio;
}