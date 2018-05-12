#include <raytracer/raytracer.h>
#include <objects/object_sphere.h>
#include <objects/object_triangle.h>
#include <materials/material.h>
#include "../utils/imageIO.cpp"
#include <cmath>
#include <lights/point_light.h>
#include <iostream>
#include <fstream>
#include <kdtree/kdtree.h>
#include <iostream>
using namespace std;

#define PI 3.14159265
#define AMBIENT_ILLUSION 1
#define LIGHT_POWER 20.0

static vec4 always_up_axis(0.0, 1.0, 0.0, 0.0f);

Raytracer::Raytracer() {
    scene = nullptr;
    current = nullptr;
    resolution_rate = 1;
}

void Raytracer::set_eye(vec4 input) {
    eye_pos = input;
}

void Raytracer::set_view_direction(vec4 input) {
    view_direction = input.normalize();
}

void Raytracer::set_fov(float input) {
    assert(0.0f < input && input < 180.0f);
    fov = input;
}
    
void Raytracer::set_resolution(int w, int h) {
    resolution[0] = w;
    resolution[1] = h;
}

void Raytracer::create_mesh() {
    if(current != nullptr) register_mesh();
    current = new Mesh();
}

void Raytracer::create_sphere(float x, float y, float z, float r) {
    Object* new_sphere = new Sphere(x, y, z, r);
    attach_object(new_sphere);
}

void Raytracer::create_triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    Object* new_triangle = new Triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
    attach_object(new_triangle);
}

void Raytracer::create_triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float n1, float n2, float n3) {
    Object* new_triangle = new Triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, n1, n2, n3);
    attach_object(new_triangle);
}

void Raytracer::resigter_material(float r, float g, float b, float ka, float kd, float ks, float exp, float refl, float refr, float nr) {
    Material temp;
    temp.color.set(r, g, b);
    temp.surface_properties.set(ka, kd, ks);
    temp.specular = exp;
    temp.scatter_ratio.set(refl, refr);
    temp.density = nr;
    current->set_material(temp);
}

void Raytracer::create_light(float x, float y, float z) {
    vec4 pos(x, y, z, 1.0f);
    vec3 color(1.0f, 1.0f, 1.0f);
    Light* new_light = new PointLight(pos, color, LIGHT_POWER);
    light_list.push_back(new_light);
}

vec3 Raytracer::get_shading(vec4 pos, vec4 view, vec4 norm, vec3 color, vec3 properties, float specular, Object* ptr) {
    //For each light, check object occlusion?
    //If not, phone shading formulate
    vec3 total_illuminance;
    total_illuminance = color * AMBIENT_ILLUSION * properties[0];
    for(size_t i = 0; i < light_list.size(); i++) {
        vec4 light_pos = light_list[i]->get_position(); 
        vec3 temp_light_pos(light_pos[0], light_pos[1], light_pos[2]);
        vec3 temp_pos(pos[0], pos[1], pos[2]);
        Ray light_ray(temp_light_pos, temp_pos - temp_light_pos, 0);
        Object* nearest = light_ray.check_nearest(tree, pos); //self occlusion?
        if(nearest == ptr) { //arrive
            float ill = (light_list[i])->get_illuminance(pos);
            float temp = norm * -light_ray.getVector();
            temp = temp <= 0.0f ? 0.0f : temp;
            total_illuminance += ( prod(color, light_list[i]->get_color()) * ill * properties[1] * ( temp ) ); //diffuse
            vec4 half_vector = (-light_ray.getVector() + -view).normalize();
            temp = half_vector * norm;
            temp = temp <= 0.0f ? 0.0f : temp;
            total_illuminance += ( light_list[i]->get_color() * ill * pow(temp, specular) * properties[2]);
        }
    }
    return total_illuminance;
}
    
void Raytracer::start_trace() {
    register_mesh();

    //left-hand coordinate

    resolution[0] *= resolution_rate;
    resolution[1] *= resolution_rate;

    create_scene();
    calculate_transform();
    execute_eye_transform();

    for(size_t i = 0; i < object_list.size(); i++) {
        object_list[i]->build_box();
    }

    float width = 2 * distance * tan(fov / 2 / 180.0 * PI);
    float height = width / resolution[0] * resolution[1];
    float width_per_pixel = width / (resolution[0] - 1);
    float height_per_pixel = height / (resolution[1] - 1);

    vec4 left_top_corner(0.0f - width / 2, 0.0f + height / 2, distance, 1.0f);
    vec4 eye_o(0.0f, 0.0f, 0.0f, 1.0f);

    sTime = clock();

    tree = new KDTree();
    tree->build_tree(object_list);

    cout << "build_finish\n";

    bTime = clock();

    cal_num = 0;

    for(int i = 0; i < resolution[1]; i++) {
        for(int j = 0; j < resolution[0]; j++) {
            vec4 now_position(left_top_corner[0] + j * width_per_pixel, left_top_corner[1] - i * height_per_pixel, left_top_corner[2], 1.0f);
            now_position -= eye_o;
            Ray tempRay(eye_o, now_position, 0);
            scene[i][j] = tempRay.trace_the_world(tree, nullptr);
        }
    }

    eTime = clock();
}

void Raytracer::output_file(char* path, char* bench) {
    ColorImage image;

    image.init(resolution[0], resolution[1]);
    for (int y=0; y<resolution[1]; y++) {
        for (int x=0; x<resolution[0]; x++) {
            Pixel p={0, 0, 0};
            if(scene[y][x][0] == 0.0f) p.R = 0;
            else if(scene[y][x][0] >= 1.0f) p.R = 255;
            else p.R = (unsigned char)floor(scene[y][x][0] * 255.0f);
            
            if(scene[y][x][1] == 0.0f) p.G = 0;
            else if(scene[y][x][1] >= 1.0f) p.G = 255;
            else p.G = (unsigned char)floor(scene[y][x][1] * 255.0f);
            
            if(scene[y][x][2] == 0.0f) p.B = 0;
            else if(scene[y][x][2] >= 1.0f) p.B = 255;
            else p.B = (unsigned char)floor(scene[y][x][2] * 255.0f); 

            assert(p.R < 256 && p.R >= 0);
            assert(p.G < 256 && p.G >= 0);
            assert(p.B < 256 && p.B >= 0);
         
            image.writePixel(x, y, p);
        }
    }
    image.outputPPM(path);

    ofstream file;
    file.open(bench, ios::out | ios::trunc);
    if(!file) {
        cout << "open file error" <<endl;
    }
    double sec = (bTime - sTime) / (double)(CLOCKS_PER_SEC);
    int hr = 0, min = 0;
    if(sec > 3600.0) {
        hr = sec / 3600.0;
        sec -= 3600.0 * hr;
    }
    if(sec > 60.0) {
        min = sec / 60.0;
        sec -= 60.0 * min;
    }
    file << "Building Duration: " << hr << ":" << min << ":" << sec << endl;
    sec = (eTime - bTime) / (double)(CLOCKS_PER_SEC);
    if(sec > 3600.0) {
        hr = sec / 3600.0;
        sec -= 3600.0 * hr;
    }
    if(sec > 60.0) {
        min = sec / 60.0;
        sec -= 60.0 * min;
    }
    file << "Traversing Duration: " << hr << ":" << min << ":" << sec << endl;
    sec = (eTime - sTime) / (double)(CLOCKS_PER_SEC);
    if(sec > 3600.0) {
        hr = sec / 3600.0;
        sec -= 3600.0 * hr;
    }
    if(sec > 60.0) {
        min = sec / 60.0;
        sec -= 60.0 * min;
    }
    file << "Total Duration: " << hr << ":" << min << ":" << sec << endl;
    file << "Primitives: " << cal_num <<endl;

    file.close();
}

Raytracer::~Raytracer() {
    for(size_t i = 0; i < mesh_list.size(); i++) {
        delete mesh_list[i];
    }
    for(size_t i = 0; i < light_list.size(); i++) {
        delete light_list[i];
    }
    for(size_t i = 0; i < object_list.size(); i++) {
        delete object_list[i];
    }
    destroy_scene();
}

void Raytracer::attach_object(Object* object) {
    current->push_back(object);
    object->set_mesh(current);
    object_list.push_back(object);
}

void Raytracer::register_mesh() {
    if(current != nullptr) {
        mesh_list.push_back(current);
        current = nullptr;
    }
}

void Raytracer::calculate_transform() {
    vec3 x_axis, y_axis;
    if(view_direction == always_up_axis) {
        x_axis.set(1.0f, 0.0f, 0.0f);
        y_axis.set(0.0f, 0.0f, -1.0f);
    }
    else if(view_direction == -always_up_axis) {
        x_axis.set(1.0f, 0.0f, 0.0f);
        y_axis.set(0.0f, 0.0f, 1.0f);
    }
    else{
        vec3 temp_view(view_direction[0], view_direction[1], view_direction[2]);
        vec3 temp_up(always_up_axis[0], always_up_axis[1], always_up_axis[2]);
        x_axis = temp_up ^ temp_view;
        y_axis = x_axis ^ (-temp_view);
    }

    vec4 temp;
    temp.set(x_axis[0], x_axis[1], x_axis[2], 0.0f);
    transform_matrix[0] = temp;
    temp.set(y_axis[0], y_axis[1], y_axis[2], 0.0f);
    transform_matrix[1] = temp;
    temp.set(view_direction[0], view_direction[1], view_direction[2], 0.0f);
    transform_matrix[2] = temp;
    temp.set(eye_pos[0], eye_pos[1], eye_pos[2], 1.0f);
    transform_matrix[3] = temp;

    transform_matrix = transform_matrix.inverse();
}

void Raytracer::execute_eye_transform() {
    for(size_t i = 0; i < object_list.size(); i++) {
        object_list[i]->updateTransform(transform_matrix);
    }
    for(size_t i = 0; i < light_list.size(); i++) {
        light_list[i]->updateTransform(transform_matrix);
    }
}

void Raytracer::create_scene() {
    if(scene != nullptr) {
        destroy_scene();
    }
    scene = new vec3*[resolution[1]];
    for(int i = 0; i < resolution[1]; i++) {
        scene[i] = new vec3[resolution[0]];
    }
}

void Raytracer::destroy_scene() {
    for(int i = 0; i < resolution[1]; i++) {
        delete [] scene[i];
    }
    delete [] scene;
    scene = nullptr;
}

void Raytracer::cal_once() {
    cal_num += 1;
}

long long int Raytracer::cal_num;

void Raytracer::set_resolution_rate(int i) {
    resolution_rate = i;
}