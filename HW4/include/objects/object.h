#ifndef OBJECT_H
#define OBJECT_H

#include <include.h>
#include <vector>
#include <utils/algebra3.h>
#include <raytracer/ray.h>
#include <utils/intersection_info.h>
#include <utils/bounding_box.h>
#include <objects/mesh.h>

class Object
{
public:
    Object() {}
    virtual ~Object() {}
    virtual float operator [](int i) = 0;
    void set_mesh(Mesh* in) {
        mesh_belong = in;
    }
    Mesh* get_mesh() {
        return mesh_belong;
    }
    virtual void updateTransform(const mat4& transform_matrix) = 0;
    virtual Intersection_info checkIntersection(Ray& nRay) = 0;
    BoundingBox get_box() {
        return mbx;
    }
    virtual void build_box() = 0;
protected:
    Mesh* mesh_belong;
    BoundingBox mbx;
};

#endif