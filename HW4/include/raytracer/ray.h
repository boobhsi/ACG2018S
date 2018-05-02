#ifndef RAY_H
#define RAY_H

#include <include.h>
#include <utils/algebra3.h>
#include <objects/mesh.h>
#include <vector>
#include <utils/intersection_info.h>
#include <kdtree/kdtree.h>
#include <kdtree/kdtree_node.h>

using namespace std;

class Ray
{
public:
    Ray(const vec4& from, const vec4& to_dir, int num);
    vec4 getOrigin();
    vec4 getVector();
    /*
    vec3 trace_the_world(const vector<Mesh*>& object_list, size_t object_index);
    vec3 trace_the_inside(const vector<Mesh*>& object_list, size_t object_index);
    */
    vec3 trace_the_world(KDTree* tree, Object* ptr);
    //vec3 trace_the_inside(KDTree* tree, Object* ptr);
    Intersection_info trace_the_node(KDTreeNode* node, Object* ptr);
    Intersection_info trace_the_tree(KDTreeNode* node, Object* ptr);
    //int check_nearest(const vector<Mesh*>& object_list, vec4 point);
    Object* check_nearest(KDTree* tree, vec4 point);
private:
    vec4 origin;
    vec4 disVector;
    int recursive_num;
    Ray get_refraction(vec4 point, float d_in, float d_out, vec4 norm);
    Ray get_reflection(vec4 point, vec4 norm);
    int get_recursive_num();
};

#endif