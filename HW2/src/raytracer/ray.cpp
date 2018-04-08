#include <raytracer/ray.h>

#define AIR_D 1.0

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

vec3 Ray::trace_the_world(const vector<Mesh*>& object_list, SHADING get_shading) {
    vec3 ray_color(0.0f, 0.0f, 0.0f), refraction_color(0.0f, 0.0f, 0.0f), reflection_color(0.0f, 0.0f, 0.0f), surface_color(0.0f, 0.0f, 0.0f);
    float refra_ratio = 0.0f, refle_ratio = 0.0f;
    float min_t = -1.0f;
    int nearest_object = 0;
    Intersection_info nearest_intersection;
    for(size_t k = 0; k < object_list.size(); k++) {
        Intersection_info intersected = object_list[k]->checkIntersection(*this);
        if(!intersected.intersected) {
            continue;
        }
        if(intersected.t > min_t) {
            nearest_intersection = intersected;
            nearest_object = k;
        }
    }
    if(min_t > 0.0f) {
        Mesh* hit_mesh = object_list[nearest_object];
        surface_color = get_shading(nearest_intersection.norm, hit_mesh->get_color(), hit_mesh->get_properties(), hit_mesh->get_specular());

        switch(nearest_intersection.surface_type) {
            case FRONT:
                Ray refraction = get_refraction(nearest_intersection.intersect_point, AIR_D, hit_mesh->get_density(), nearest_intersection.norm);
                if(refraction.get_recursive_num >= 0) { // no completed reflection
                    refraction_color = refraction.trace_the_inside(hit_mesh, object_list, get_shading);
                }
                refra_ratio = hit_mesh->get_refraction_ratio();
                Ray reflection = get_reflection(nearest_intersection.intersect_point, nearest_intersection.norm);
                reflection_color = reflection.trace_the_world(object_list, get_shading);
                refle_ratio = hit_mesh->get_reflection_ratio();
                break;
                /*
            case BACK:
                Ray refraction = get_refraction();
                ray_color = refraction.trace_the_world(object_list);
                break;
                */
            case RIGID:
                Ray reflection = get_reflection(nearest_intersection.intersect_point, nearest_intersection.norm);
                reflection_color = reflection.trace_the_world(object_list, get_shading);
                refle_ratio = hit_mesh->get_reflection_ratio();
                break;
            default:
                break;
        }
    }
    ray_color = refra_ratio * refraction_color + refle_ratio + reflection_color + (1 - refle_ratio - refra_ratio) * surface_color;
    return ray_color;
}

Ray Ray::get_refraction(vec4 point, float d_in, float d_out, vec4 norm) {
    float c = norm * disVector;
    float s_2 = (1 - c * c) * pow(d_in / d_out, 2.0);
    if(s_2 > 1) {
        vec4 temp(1.0f, 0.0f, 0.0f, 0.0f); //fake
        Ray new_ray(point, temp, -1);
        return new_ray;
    }
    else {
        vec4 temp = (d_in / d_out) * disVector + (d_in / d_out * c - sqrt(1.0 - s_2)) * norm;
        Ray new_ray(point, temp, recursive_num + 1);
        return new_ray;
    }
}

Ray Ray::get_reflection(vec4 point, vec4 norm) {
    vec4 mir = ( disVector * norm ) / norm.length() * norm;
    vec4 dis = disVector - mir;
    vec4 new_dir = ( 2 * dis ) - disVector;
    Ray new_ray(point, new_dir, recursive_num + 1);
    return new_ray;
}

vec3 Ray::trace_the_inside(Mesh* object_inside, const vector<Mesh*>& object_list, SHADING get_shading) {
    vec3 ray_color(0.0f, 0.0f, 0.0f);
    Intersection_info intersection = object_inside->checkIntersection(*this);
    Ray refraction = get_refraction(intersection.intersect_point, object_inside->get_density(), AIR_D, intersection.norm);
    if(refraction.get_recursive_num() >= 0) { // no completed reflection
        ray_color = refraction.trace_the_world(object_list, get_shading);
    }
    return ray_color;
}

    int get_recursive_num();