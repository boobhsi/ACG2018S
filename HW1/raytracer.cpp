#include "raytracer.h"
#include "utils/object_sphere.h"
#include "utils/object_triangle.h"
#include "utils/imageIO.cpp"
#include <cmath>

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

#define PI 3.14159265

static vec3 always_up_axis(0.0, 1.0, 0.0);

Raytracer::Raytracer() {
    scene = nullptr;
}

void Raytracer::set_eye(vec3 input) {
   
#ifdef DEBUG
    cout << "set_eye (" << input[0] << ", " << input[1] << ", " << input[2] << ")" << endl;
#endif

    eye_pos = input;
    Ray::setOrigin(eye_pos);
}

void Raytracer::set_view_direction(vec3 input) {

#ifdef DEBUG
    cout << "set_view_direction (" << input[0] << ", " << input[1] << ", " << input[2] << ")" << endl;
#endif

    view_direction = input.normalize();
}

void Raytracer::set_fov(float input) {

#ifdef DEBUG
    cout << "set_fov (" << input << ")" << endl;
#endif

    assert(0.0f < input && input < 180.0f);
    fov = input;
}
    
void Raytracer::set_resolution(int w, int h) {

#ifdef DEBUG
    cout << "set_resolution (" << w << ", " << h << ")" << endl;
#endif

    resolution[0] = w;
    resolution[1] = h;
}
    
void Raytracer::start_trace() {

    //left-hand coordinate

    vec3 x_axis = always_up_axis ^ view_direction;
    vec3 y_axis = x_axis ^ (-view_direction);

#ifdef DEBUG
    cout << "x_axis: (" << x_axis[0] << ", " << x_axis[1] << ", " << x_axis[2] << ")\n";
    cout << "y_axis: (" << y_axis[0] << ", " << y_axis[1] << ", " << y_axis[2] << ")\n";
#endif

    float width = 2 * distance * tan(fov / 2 / 180.0 * PI);
    float height = width / resolution[0] * resolution[1];
    float width_per_pixel = width / (resolution[0] - 1);
    float height_per_pixel = height / (resolution[1] - 1);

#ifdef DEBUG
    cout << "width: " << width << ", height: " << height << ", width per pixel: " << width_per_pixel << ", height per pixel: " << height_per_pixel <<"\n";
#endif

    vec3 scene_center = eye_pos + view_direction * distance;
    vec3 left_top_corner = scene_center - width / 2 * x_axis + height / 2 * y_axis;

#ifdef DEBUG
    cout << "lt: (" << left_top_corner[0] << ", " << left_top_corner[1] << ", " << left_top_corner[2] << ")\n";
#endif

    for(int i = 0; i < resolution[1]; i++) {
        for(int j = 0; j < resolution[0]; j++) {
            vec3 now_position = left_top_corner - i * height_per_pixel * y_axis + j * width_per_pixel * x_axis;
            Ray tempRay(now_position);
            bool intersected = false;
            for(size_t k = 0; k < object_list.size(); k++) {
                intersected = object_list[k]->checkIntersection(tempRay);
                if(intersected) break;
            }
            scene[i][j] = intersected;
        }
    }
}

void Raytracer::create_scene() {
    scene = new bool*[resolution[1]];
    for(int i = 0; i < resolution[1]; i++) {
        scene[i] = new bool[resolution[0]];
    }
}

vec3 Raytracer::eye_transform(vec3 pos) {
    //mistaken: no need to do anything
    return pos;
}

void Raytracer::register_object(Object* object) {
    object_list.push_back(object);
}

Raytracer::~Raytracer() {
    for(size_t i = 0; i < object_list.size(); i++) {
        delete object_list[i];
    }
    if(scene != nullptr) {
        for(int i = 0; i < resolution[1]; i++) delete [] scene[i];
        delete [] scene;
        scene = nullptr;
    }
}

void Raytracer::create_triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    vec3 point1(x1, y1, z1); 
    vec3 point2(x2, y2, z2);
    vec3 point3(x3, y3, z3);
    point1 = eye_transform(point1);
    point2 = eye_transform(point2);
    point3 = eye_transform(point3);
    current = new Triangle(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2], point3[0], point3[1], point3[2]);
    register_object(current);
}

void Raytracer::create_sphere(float x, float y, float z, float r) {
    vec3 s_center(x, y, z);
    s_center = eye_transform(s_center);
    current = new Sphere(s_center[0], s_center[1], s_center[2], r);
    register_object(current);
}

void Raytracer::output_file(char* path) {
    ColorImage image;
 
    image.init(resolution[0], resolution[1]);
    for (int y=0; y<resolution[1]; y++) {
        for (int x=0; x<resolution[0]; x++) {
            Pixel p={0, 0, 0};
            if(scene[y][x] == true) {
                p.R = 255;
                p.G = 255;
                p.B = 255;
            }
            image.writePixel(x, y, p);
        }
    }
    image.outputPPM(path);
}