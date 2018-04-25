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
    Intersection_info checkIntersection(Ray& nray);
    BlockSet slash(int axis, float pos);
    void extend(BoundingBox bx);
    void init_box();
    void draw();
    bool isInside(Ray& nray);
private:
    void init(vec4 p1, vec4 p2, vec4 p3, vec4 p4, vec4 p5, vec4 p6, vec4 p7, vec4 p8);
    static const int BLOCK_TRIANGLE_NUM = 12;
    Triangle** boundary;
    vec4 points[8];
    BoundingBox mbx;
};

#endif