#include <raytracer/raytracer.h>
#include <objects/object_sphere.h>
#include <objects/object_triangle.h>
#include <materials/material.h>
#include "../utils/imageIO.cpp"
#include <cmath>
#include <lights/point_light.h>
#include <iostream>
#include <fstream>
using namespace std;

#define PI 3.14159265
#define AMBIENT_ILLUSION 1
#define LIGHT_POWER 20.0

static vec3 always_up_axis(0.0, 1.0, 0.0);

Raytracer::Raytracer() {
    scene = nullptr;
    current = nullptr;
}

void Raytracer::set_eye(vec3 input) {
   
#ifdef DEBUG
    cout << "set_eye (" << input[0] << ", " << input[1] << ", " << input[2] << ")" << endl;
#endif

    eye_pos = input;
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

void Raytracer::create_mesh() {
#ifdef DEBUG
    cout << "create mesh\n";
#endif
    if(current != nullptr) register_mesh();
    current = new Mesh();
}

void Raytracer::create_sphere(float x, float y, float z, float r) {
    vec3 s_center(x, y, z);
    /*
    s_center = eye_transform(s_center);
    */
    Object* new_sphere = new Sphere(s_center[0], s_center[1], s_center[2], r);
    attach_object(new_sphere);
}

void Raytracer::create_triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    vec3 point1(x1, y1, z1); 
    vec3 point2(x2, y2, z2);
    vec3 point3(x3, y3, z3);
    /*
    point1 = eye_transform(point1);
    point2 = eye_transform(point2);
    point3 = eye_transform(point3);
    */
    Object* new_triangle = new Triangle(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2], point3[0], point3[1], point3[2]);
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
#ifdef DEBUG
    cout << "insert material: (" << r << ", " << g << ", " << b << ", " << ka << ", " << kd << ", " << ks << ", " << exp << ", " << refl << ", " << refr << ", " << nr << ")" <<"\n";
#endif
}

void Raytracer::create_light(float x, float y, float z) {
    vec4 pos(x, y, z, 1.0f);
    vec3 color(1.0f, 1.0f, 1.0f);
    Light* new_light = new PointLight(pos, color, LIGHT_POWER);
    light_list.push_back(new_light);

    #ifdef DEBUG
    cout << "create light: (" << x << ", " << y << ", " << z << ")" <<"\n";
    #endif
}

vec3 Raytracer::get_shading(vec4 pos, vec4 view, vec4 norm, vec3 color, vec3 properties, float specular, int object_index) {
    //For each light, check object occlusion?
    //If not, phone shading formulate
    vec3 total_illuminance;
    total_illuminance = color * AMBIENT_ILLUSION * properties[0];
    for(size_t i = 0; i < light_list.size(); i++) {
        vec4 light_pos = light_list[i]->get_position(); 
        vec3 temp_light_pos(light_pos[0], light_pos[1], light_pos[2]);
        vec3 temp_pos(pos[0], pos[1], pos[2]);
        Ray light_ray(temp_light_pos, temp_pos - temp_light_pos, 0);
        int nearest = light_ray.check_nearest(mesh_list, pos); //self occlusion?
        if(nearest == object_index) { //arrive
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
    
#ifdef DEBUG
    cout << "mesh count: " << mesh_list.size() << endl;
    cout << "light count: " << light_list.size() << endl;
#endif

    //left-hand coordinate



    create_scene();
    calculate_transform();
    execute_eye_transform();

    float width = 2 * distance * tan(fov / 2 / 180.0 * PI);
    float height = width / resolution[0] * resolution[1];
    float width_per_pixel = width / (resolution[0] - 1);
    float height_per_pixel = height / (resolution[1] - 1);

#ifdef DEBUG
    cout << "width: " << width << ", height: " << height << ", width per pixel: " << width_per_pixel << ", height per pixel: " << height_per_pixel <<"\n";
#endif

    vec3 left_top_corner(0.0f - width / 2, 0.0f + height / 2, distance);
    vec3 eye_o(0.0f, 0.0f, 0.0f);

#ifdef DEBUG
    cout << "lt: (" << left_top_corner[0] << ", " << left_top_corner[1] << ", " << left_top_corner[2] << ")\n";
#endif

    cal_num = 0;
    sTime = clock();

    for(int i = 0; i < resolution[1]; i++) {
        for(int j = 0; j < resolution[0]; j++) {
            vec3 now_position(left_top_corner[0] + j * width_per_pixel, left_top_corner[1] - i * height_per_pixel, left_top_corner[2]);
            Ray tempRay(eye_o, now_position, 0);
            /*
            bool intersected = false;
            for(size_t k = 0; k < object_list.size(); k++) {
                intersected = object_list[k]->checkIntersection(tempRay);
                if(intersected) break;
            }
            */
            scene[i][j] = tempRay.trace_the_world(mesh_list, mesh_list.size());
        }
    }

    eTime = clock();

#ifdef DEBUG
    cout << "trace over\n";
#endif   
}

void Raytracer::output_file(char* path) {
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
            

            //p.R = (unsigned char)(255.0f * min(scene[y][x][0], 1.0f));
            //p.G = (unsigned char)(255.0f * min(scene[y][x][1], 1.0f));
            //p.B = (unsigned char)(255.0f * min(scene[y][x][2], 1.0f));
#ifdef DEBUG
    //cout << p.R << ", " << p.G << ", " << p.B << endl;
#endif            
            image.writePixel(x, y, p);
        }
    }
    image.outputPPM(path);
#ifdef DEBUG
    vec3 temp(0.0, 0.0, 0.0);
    vec4 temp4(0.0, 0.0, 0.0, 0.0);
    vec4 temp_norm(0.0, 1.0, 0.0, 0.0);
    vec3 temp_ray(1.0, -1.0, 0.0);
    Ray test_refraction(temp, temp_ray, 0);
    Ray new_temp = test_refraction.get_refraction(temp4, 1.0, 2.0, temp_norm);
    cout << "test_result: (" << new_temp.getVector()[0] << ", " << new_temp.getVector()[1] << ", " << new_temp.getVector()[2] << ", " << new_temp.getVector()[3] << ")\n";
#endif

    ofstream file;
    file.open("benchmark.txt", ios::out | ios::trunc);
    if(!file) {
        cout << "open file error" <<endl;
    }
    double sec = (eTime - sTime) / (double)(CLOCKS_PER_SEC);
    int hr = 0, min = 0;
    if(sec > 3600.0) {
        hr = sec / 3600.0;
        sec -= 3600.0 * hr;
    }
    if(sec > 60.0) {
        min = sec / 60.0;
        sec -= 60.0 * min;
    }
    file << "Duration: " << hr << ":" << min << ":" << sec << endl;
    file << "Primitives: " << cal_num <<endl;
}

Raytracer::~Raytracer() {
    for(size_t i = 0; i < mesh_list.size(); i++) {
        delete mesh_list[i];
    }
    for(size_t i = 0; i < light_list.size(); i++) {
        delete light_list[i];
    }
    destroy_scene();
}

void Raytracer::attach_object(Object* object) {
    current->push_back(object);
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
        x_axis = always_up_axis ^ view_direction;
        y_axis = x_axis ^ (-view_direction);
    }

#ifdef DEBUG
    cout << "x_axis: (" << x_axis[0] << ", " << x_axis[1] << ", " <<x_axis[2] << ")\n";
    cout << "y_axis: (" << y_axis[0] << ", " << y_axis[1] << ", " <<y_axis[2] << ")\n";
#endif

    vec4 temp;
    temp.set(x_axis[0], x_axis[1], x_axis[2], 0.0f);
    transform_matrix[0] = temp;
    temp.set(y_axis[0], y_axis[1], y_axis[2], 0.0f);
    transform_matrix[1] = temp;
    temp.set(view_direction[0], view_direction[1], view_direction[2], 0.0f);
    transform_matrix[2] = temp;
    temp.set(eye_pos[0], eye_pos[1], eye_pos[2], 1.0f);
    transform_matrix[3] = temp;

#ifdef DEBUG
    cout << transform_matrix[0][0] << ", " << transform_matrix[0][1] << ", " << transform_matrix[0][2] << ", " << transform_matrix[0][3] << ")\n";
    cout << transform_matrix[1][0] << ", " << transform_matrix[1][1] << ", " << transform_matrix[1][2] << ", " << transform_matrix[1][3] << ")\n";
    cout << transform_matrix[2][0] << ", " << transform_matrix[2][1] << ", " << transform_matrix[2][2] << ", " << transform_matrix[2][3] << ")\n";
    cout << transform_matrix[3][0] << ", " << transform_matrix[3][1] << ", " << transform_matrix[3][2] << ", " << transform_matrix[3][3] << ")\n";
#endif

    transform_matrix = transform_matrix.inverse();

#ifdef DEBUG
    cout << transform_matrix[0][0] << ", " << transform_matrix[0][1] << ", " << transform_matrix[0][2] << ", " << transform_matrix[0][3] << ")\n";
    cout << transform_matrix[1][0] << ", " << transform_matrix[1][1] << ", " << transform_matrix[1][2] << ", " << transform_matrix[1][3] << ")\n";
    cout << transform_matrix[2][0] << ", " << transform_matrix[2][1] << ", " << transform_matrix[2][2] << ", " << transform_matrix[2][3] << ")\n";
    cout << transform_matrix[3][0] << ", " << transform_matrix[3][1] << ", " << transform_matrix[3][2] << ", " << transform_matrix[3][3] << ")\n";
#endif
}

void Raytracer::execute_eye_transform() {
    for(size_t i = 0; i < mesh_list.size(); i++) {
        mesh_list[i]->updateTransform(transform_matrix);
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