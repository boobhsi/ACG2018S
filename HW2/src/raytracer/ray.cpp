#include <raytracer/ray.h>
#include <iostream>

#define AIR_D 1.0
#define SAME_POINT_THRESHOLD 0.0001
#define OFFSET 0.0001

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

extern Raytracer* rt;

/*
vec4 Ray::origin;

void Ray::setOrigin(vec3& input) {
    Ray::origin[0] = input[0];
    Ray::origin[1] = input[1];
    Ray::origin[2] = input[2];
    Ray::origin[3] = 1.0f;
}
*/

vec4 Ray::getOrigin() {
    return Ray::origin;
}

Ray::Ray(const vec3& from, const vec3& to_dir, int num) {
    vec4 temp_o(from, 1.0f);
    origin = temp_o;
    vec3 temp_d = to_dir;
    vec4 temp(temp_d.normalize(), 0.0f);
    //disVector = temp - origin;
    disVector = temp;
    recursive_num = num;
}

vec4 Ray::getVector() {
    return disVector;
}

vec3 Ray::trace_the_world(const vector<Mesh*>& object_list, size_t object_index) {
    vec3 ray_color(0.0f, 0.0f, 0.0f), refraction_color(0.0f, 0.0f, 0.0f), reflection_color(0.0f, 0.0f, 0.0f), surface_color(0.0f, 0.0f, 0.0f);
    vec2 scatter_ratio(0.0f, 0.0f);
    float min_t = -1.0f;
    bool have_intersected = false;
    int nearest_object = 0;
    Intersection_info nearest_intersection;
    for(size_t k = 0; k < object_list.size(); k++) {
        if(k == object_index) continue;
#ifdef DEBUG
    cout << "ray (" << disVector[0] << ", " << disVector[1] << ", " << disVector[2] << ", " << disVector[3] << ")" << endl;
#endif
        Intersection_info intersected = object_list[k]->checkIntersection(*this);
        if(!intersected.intersected) {
            continue;
        }
        else if(!have_intersected) {
            nearest_intersection = intersected;
            nearest_object = k;
            min_t = intersected.t;
            have_intersected = true;
        }
        else if(intersected.t < min_t) {
            nearest_intersection = intersected;
            min_t = intersected.t;
            nearest_object = k;
        }
    }
    if(have_intersected) {
        Mesh* hit_mesh = object_list[nearest_object];
        surface_color = rt->get_shading(nearest_intersection.intersect_point, this->getVector(), nearest_intersection.norm, hit_mesh->get_color(), hit_mesh->get_properties(), hit_mesh->get_specular(), nearest_object);
        scatter_ratio = hit_mesh->get_scatter_ratio();
        if(this->get_recursive_num() < RECURSIVE_LIMIT) {
            switch(nearest_intersection.surface_type) {
                case FRONT: {
                    Ray refraction = get_refraction(nearest_intersection.intersect_point, AIR_D, hit_mesh->get_density(), nearest_intersection.norm);
                    if(refraction.get_recursive_num() >= 0) { // no completed reflection
                        refraction_color = refraction.trace_the_inside(object_list, nearest_object);
                    }
                    Ray reflection = get_reflection(nearest_intersection.intersect_point, nearest_intersection.norm);
                    reflection_color = reflection.trace_the_world(object_list, nearest_object);
                    break;
                }
                    /*
                case BACK:
                    Ray refraction = get_refraction();
                    ray_color = refraction.trace_the_world(object_list);
                    break;
                    */
                case RIGID: {
                    Ray reflection = get_reflection(nearest_intersection.intersect_point, nearest_intersection.norm);
                    reflection_color = reflection.trace_the_world(object_list, nearest_object);
                    break;
                }
                default:
                    break;
            }
        }
    }
    ray_color = (scatter_ratio[0] * reflection_color) + (scatter_ratio[1] * refraction_color) + (  surface_color);
    return ray_color;
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
#ifdef DEBUG
    cout << "get reflection from (" << disVector[0] << ", " << disVector[1] << ", " << disVector[2] << ", " << disVector[3] << ")" << endl;
    cout << "norm (" << norm[0] << ", " << norm[1] << ", " << norm[2] << ", " << norm[3] << ")"<< endl;
#endif
    vec4 mir = ( disVector * norm ) / norm.length() * norm;
#ifdef DEBUG
    cout << "mir to norm (" << mir[0] << ", " << mir[1] << ", " << mir[2] << ", " << mir[3] << ")" << endl;
#endif    
    vec4 dis = disVector - mir;
#ifdef DEBUG
    cout << "mir to dis (" << dis[0] << ", " << dis[1] << ", " << dis[2] << ", " << dis[3] << ")" << endl;
#endif    
    vec4 new_dir = ( 2 * dis ) - disVector;
#ifdef DEBUG
    cout << "mir to dis (" << new_dir[0] << ", " << new_dir[1] << ", " << new_dir[2] << ", " << new_dir[3] << ")" << endl;
#endif
    vec3 temp(new_dir[0], new_dir[1], new_dir[2]);
    vec3 temp_p(point[0], point[1], point[2]);
    Ray new_ray(temp_p + temp * OFFSET, temp, recursive_num + 1);
#ifdef DEBUG
    cout << "result (" << new_ray.getVector()[0] << ", " << new_ray.getVector()[1] << ", " << new_ray.getVector()[2] << ", " << new_ray.getVector()[3] << ")"<< endl;
#endif
    return new_ray;
}

vec3 Ray::trace_the_inside(const vector<Mesh*>& object_list, size_t object_index) {
    Mesh* object_inside = object_list[object_index];
    vec3 ray_color(0.0f, 0.0f, 0.0f);
    Intersection_info intersection = object_inside->checkIntersection(*this);
    Ray refraction = get_refraction(intersection.intersect_point, object_inside->get_density(), AIR_D, intersection.norm);
    if(refraction.get_recursive_num() >= 0) { // no completed reflection
        ray_color = refraction.trace_the_world(object_list, object_index);
    }
    return ray_color;
}

int Ray::get_recursive_num() {
    return recursive_num;
}

int Ray::check_nearest(const vector<Mesh*>& object_list, vec4 point) {
    float min_t = -1.0f;
    int nearest_object = 0;
    bool have_intersected = false;
    Intersection_info nearest_intersection;
    for(size_t k = 0; k < object_list.size(); k++) {
        Intersection_info intersected = object_list[k]->checkIntersection(*this);
        if(!intersected.intersected) {
            continue;
        }
        else if (!have_intersected) {
            nearest_intersection = intersected;
            nearest_object = k;
            min_t = intersected.t;
            have_intersected = true;
        }
        else if(intersected.t < min_t) {
            nearest_intersection = intersected;
            nearest_object = k;
            min_t = intersected.t;
        }
    }
    if(have_intersected && (nearest_intersection.intersect_point - point).length() < SAME_POINT_THRESHOLD) {
        return nearest_object;
    }
    else{
        return -1;
    }
}