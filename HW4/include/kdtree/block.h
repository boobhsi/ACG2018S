#ifndef BLOCK_H
#define BLOCK_H

#include <include.h>
#include <raytracer/ray.h>
#include <utils/algebra3.h>
#include <objects/object_triangle.h>
#include <utils/bounding_box.h>
#include <utils/intersection_info.h>

typedef std::pair<Block*, Block*> BlockSet;

class Block {
public:
    Block(vec4 p1, vec4 p2, vec4 p3, vec4 p4, vec4 p5, vec4 p6, vec4 p7, vec4 p8);
    Block(BoundingBox bx);
    ~Block();
    Intersection_info checkIntersection(Ray& nray);
    BlockSet slash(int axis, float pos);
    void extend(BoundingBox bx);
    void init_box();
    void draw();
    bool isInside(Ray& nray);
    float get_max_x();
    float get_min_x();
    float get_max_y();
    float get_min_y();
    float get_max_z();
    float get_min_z();
    vec4 get_center();
    float get_height();
    float get_width();
    float get_depth();
    float get_area();
private:
    void init(vec4 p1, vec4 p2, vec4 p3, vec4 p4, vec4 p5, vec4 p6, vec4 p7, vec4 p8);
    static const int BLOCK_TRIANGLE_NUM = 12;
    Triangle** boundary;
    vec4 points[8];
    BoundingBox mbx;
    float area;
};

#endif