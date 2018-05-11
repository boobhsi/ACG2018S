#include <raytracer/ray.h>
#include <raytracer/raytracer.h>
#include <iostream>
#include <constants.h>

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

extern Raytracer* rt;

//static const string DEBUG_FLAG = "nlr";

vec4 Ray::getOrigin() {
    return Ray::origin;
}

Ray::Ray(const vec4& from, const vec4& to_dir, int num) {
    origin = from;
    disVector = to_dir;
    disVector = disVector.normalize();
    recursive_num = num;
}

vec4 Ray::getVector() {
    return disVector;
}

Ray Ray::get_refraction(vec4 point, float d_in, float d_out, vec4 norm) {
    float c = -(norm * disVector);
    float s_2 = (1.0f - c * c) * pow(d_in / d_out, 2.0);
    if(s_2 > 1) {
        vec3 temp(1.0f, 0.0f, 0.0f); //fake
        vec3 t(point[0], point[1], point[2]);
        Ray new_ray(t, temp, -1);
        return new_ray;
    }
    else {
        vec4 temp = ((d_in / d_out) * disVector) + ((d_in / d_out * c - sqrt(1.0 - s_2)) * norm);
        vec3 t(point[0], point[1], point[2]);
        vec3 tt(temp[0], temp[1], temp[2]);
        Ray new_ray(t + tt * OFFSET, tt, recursive_num);
        return new_ray;
    }
}

Ray Ray::get_reflection(vec4 point, vec4 norm) {
    vec4 mir = ( disVector * norm ) / norm.length() * norm;  
    vec4 dis = disVector - mir;  
    vec4 new_dir = ( 2 * dis ) - disVector;
    vec3 temp(new_dir[0], new_dir[1], new_dir[2]);
    vec3 temp_p(point[0], point[1], point[2]);
    Ray new_ray(temp_p + temp * OFFSET, temp, recursive_num + 1);
    return new_ray;
}

int Ray::get_recursive_num() {
    return recursive_num;
}

Intersection_info Ray::trace_the_node(KDTreeNode* node, Object* ptr) {
    float min_t = -1.0f;
    bool have_intersected = false;
    Intersection_info nearest_intersection;
    if(node->get_block().checkIntersection(*this).intersected) {
        vector<Object*>& object_list = node->get_objects();
        for(size_t k = 0; k < object_list.size(); k++) {
            if(object_list[k] == ptr) continue;
            Intersection_info intersected = object_list[k]->checkIntersection(*this);
            if(!intersected.intersected) {
                continue;
            }
            else if(!have_intersected) {
                nearest_intersection = intersected;
                min_t = intersected.t;
                have_intersected = true;
            }
            else if(intersected.t < min_t) {
                nearest_intersection = intersected;
                min_t = intersected.t;
            }
        }
    }
    return nearest_intersection;
}

Intersection_info Ray::trace_the_tree(KDTreeNode* node, Object* ptr) {
    Intersection_info nearest_intersection, block_nearest_intersection;

    if(node->is_leaf()) {
//        if(node->getFlag().compare(DEBUG_FLAG) != 0) {
//            return nearest_intersection;
//        }
        nearest_intersection = trace_the_node(node, ptr);
    }
    else {

//            nearest_intersection = trace_the_tree(node->get_left(), ptr);
//        block_nearest_intersection = trace_the_tree(node->get_right(), ptr);
//        if(!nearest_intersection.intersected) return block_nearest_intersection;
//        else if(!block_nearest_intersection.intersected) return nearest_intersection;
//        else {
//            if(nearest_intersection.t < block_nearest_intersection.t) return nearest_intersection;
//            else return block_nearest_intersection;
//        }


        Intersection_info l_intersected, r_intersected;
        block_nearest_intersection = node->get_left()->get_block().checkIntersection(*this);
        if(!block_nearest_intersection.intersected) { //doesnt hit left block
            block_nearest_intersection = node->get_right()->get_block().checkIntersection(*this);
            if(block_nearest_intersection.intersected) { //hit right block
                r_intersected = trace_the_tree(node->get_right(), ptr);
                nearest_intersection = r_intersected;
            }
        }
        else { //hit left block
            float t_l = block_nearest_intersection.t;
            block_nearest_intersection = node->get_right()->get_block().checkIntersection(*this);
            if(!block_nearest_intersection.intersected) { //doesnt hit right block
                l_intersected = trace_the_tree(node->get_left(), ptr);
                nearest_intersection = l_intersected;
            }
            else { //hit right block
                float t_r = block_nearest_intersection.t;
               // bool li = node->get_left()->get_block().isInside(*this);
               // bool ri = node->get_right()->get_block().isInside(*this);
                if(t_l > t_r) {
                    r_intersected = trace_the_tree(node->get_right(), ptr); 
                    if(!r_intersected.intersected) {
                        l_intersected = trace_the_tree(node->get_left(), ptr);
                        nearest_intersection = l_intersected;
                    }
                    else {
                        /*
                        if(li) {
                            l_intersected = trace_the_tree(node->get_left(), ptr);
                            if(l_intersected.intersected) {
                                if(r_intersected.t > l_intersected.t) {
                                    nearest_intersection = l_intersected;
                                }
                                else {
                                    nearest_intersection = r_intersected;
                                }
                            }
                            else {
                                nearest_intersection = r_intersected;
                            }
                        }
                        else */
                        if(t_l < r_intersected.t) {
                            l_intersected = trace_the_tree(node->get_left(), ptr);
                            if(l_intersected.intersected) {
                                if(r_intersected.t > l_intersected.t) {
                                    nearest_intersection = l_intersected;
                                }
                                else {
                                    nearest_intersection = r_intersected;
                                }
                            }
                            else {
                                nearest_intersection = r_intersected;
                            }
                        }
                        else {
                            
                            nearest_intersection = r_intersected;
                        }
                    }
                }
                else {
                    l_intersected = trace_the_tree(node->get_left(), ptr);
                    if(!l_intersected.intersected) {
                        r_intersected = trace_the_tree(node->get_right(), ptr);
                        nearest_intersection = r_intersected;
                    }
                    else {
                        /*
                        if(ri) {
                            r_intersected = trace_the_tree(node->get_right(), ptr);
                            if(r_intersected.intersected) {
                                if(r_intersected.t > l_intersected.t) {
                                    nearest_intersection = l_intersected;
                                }
                                else {
                                    nearest_intersection = r_intersected;
                                }
                            }
                            else {
                                nearest_intersection = l_intersected;
                            }
                        }
                        else */
                        if(t_r < l_intersected.t) {
                            r_intersected = trace_the_tree(node->get_right(), ptr);
                            if(r_intersected.intersected) {
                                if(r_intersected.t > l_intersected.t) {
                                    nearest_intersection = l_intersected;
                                }
                                else {
                                    nearest_intersection = r_intersected;
                                }
                            }
                            else {
                                nearest_intersection = l_intersected;
                            }
                        }
                        else {
                            
                            nearest_intersection = l_intersected;
                        }
                    }
                }
            }
        }      
    }
    return nearest_intersection;
}

/*
vec3 Ray::trace_the_inside(KDTree* tree, Object* ptr) {
    Mesh* mesh_inside = ptr->get_mesh();
    vec3 ray_color(0.0f, 0.0f, 0.0f);
    KDTree* mesh_tree = mesh_inside->get_tree();
    Intersection_info intersection =trace_the_tree(mesh_tree->get_root(), ptr);
    Ray refraction = get_refraction(intersection.intersect_point, mesh_inside->get_density(), AIR_D, intersection.norm);
    if(refraction.get_recursive_num() >= 0) { // no completed reflection
        ray_color = refraction.trace_the_world(tree, intersection.surface);
    }
    return ray_color;
}
*/

vec3 Ray::trace_the_world(KDTree* tree, Object* ptr) {
    vec3 ray_color(0.0f, 0.0f, 0.0f), refraction_color(0.0f, 0.0f, 0.0f), reflection_color(0.0f, 0.0f, 0.0f), surface_color(0.0f, 0.0f, 0.0f);
    vec2 scatter_ratio(0.0f, 0.0f);
    Intersection_info block_intersection = tree->get_root()->get_block().checkIntersection(*this);
    Intersection_info nearest_intersection;
    if(block_intersection.intersected) {
        nearest_intersection = trace_the_tree(tree->get_root(), ptr);
    }
    if(nearest_intersection.intersected) {
        Mesh* hit_mesh = nearest_intersection.surface->get_mesh();
        surface_color = rt->get_shading(nearest_intersection.intersect_point, this->getVector(), nearest_intersection.norm, hit_mesh->get_color(), hit_mesh->get_properties(), hit_mesh->get_specular(), nearest_intersection.surface);
        scatter_ratio = hit_mesh->get_scatter_ratio();
        if(this->get_recursive_num() < RECURSIVE_LIMIT) {
            switch(nearest_intersection.surface_type) {
                case FRONT: {
                    /*
                    Ray refraction = get_refraction(nearest_intersection.intersect_point, AIR_D, hit_mesh->get_density(), nearest_intersection.norm);
                    if(refraction.get_recursive_num() >= 0) { // no completed reflection
                        refraction_color = refraction.trace_the_inside(tree, nearest_intersection.surface);
                    }
                    */
                    Ray reflection = get_reflection(nearest_intersection.intersect_point, nearest_intersection.norm);
                    reflection_color = reflection.trace_the_world(tree, nearest_intersection.surface);
                    break;
                }
                /*
                case BACK: {
                    Ray refraction = get_refraction();
                    ray_color = refraction.trace_the_world(tree, nearest_intersection.surface);
                    break;
                }
                */
                case RIGID: {
                    Ray reflection = get_reflection(nearest_intersection.intersect_point, nearest_intersection.norm);
                    reflection_color = reflection.trace_the_world(tree, nearest_intersection.surface);
                    break;
                }
                default:
                    break;
            }
        }
    }
    ray_color = (scatter_ratio[0] * reflection_color) + (scatter_ratio[1] * refraction_color) + (surface_color);
    return ray_color;
}

Object* Ray::check_nearest(KDTree* tree, vec4 point) {
    Intersection_info nearest_intersection = trace_the_tree(tree->get_root(), nullptr);
    vec3 t_p(point[0], point[1], point[2]);
    vec3 t_n(nearest_intersection.intersect_point[0], nearest_intersection.intersect_point[1], nearest_intersection.intersect_point[2]);
    if(nearest_intersection.intersected && (t_n - t_p).length() < SAME_POINT_THRESHOLD) {
        return nearest_intersection.surface;
    }
    else{
        return nullptr;
    }
}