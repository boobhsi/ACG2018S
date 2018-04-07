#include <objects/mesh.h>
#include <assert>

using namespace std;

Mesh::Mesh() {
    rigid = false;
}

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
    if(mMaterial.density == 0) {
        rigid = true;
    }
}

Intersection_info Mesh::checkIntersection(Ray& nray) {

    assert(object_list.size() != 0);

    //find nearest object, return nearest surface
    Intersection_info nearest_intersect = object_list[0]->checkIntersection(nray);
    if(object_list.size() > 1) {
        for(size_t i = 1; i < object_list[i]; i++) {
            Intersection_info temp_intersect = object_list[i]->checkIntersection(nRay);
            if(!temp_intersect.intersected) {
                continue;
            }
            else{
                if(!nearest_intersect.intersected) {
                    nearest_intersect = temp_intersect;
                }
                else if(temp_intersect.t < nearest_intersect) {
                    nearest_intersect = temp_intersect;
                }
                else {
                    continue;
                }
            }
        }
    }
    if(rigid) {
        nearest_intersect.surface_type = RIGID;
    }
    return nearest_intersect;
}
    
float Mesh::get_density() {
    return mMaterial.density;
}