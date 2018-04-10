#include <iostream>
#include <fstream>
#include <vector>
#include "utils/parse_func.cpp"
#include <raytracer/raytracer.h>
#include <string>

using namespace std;

Raytracer* rt;

int main(int argc, char* argv[]) {
    rt = new Raytracer();;
    fstream input_file;
    input_file.open(argv[1], ios::in);
    char temp_char[200];
    while(input_file.getline(temp_char, 200)) {
        string temp_string(temp_char);
        if(temp_string.size() == 0) {
            continue;
        }
        temp_string = temp_string.substr(1);
        size_t c_pos = temp_string.find_first_not_of(' ');
        size_t lc_pos = temp_string.find_last_not_of(' ');
        if(c_pos == string::npos || lc_pos == string::npos) {
            continue;
        }
        temp_string = temp_string.substr(c_pos, lc_pos - c_pos + 1);
        switch(temp_char[0]) {
            case 'E':
            {
                vector<float> temp = get_float(temp_string);
                vec3 tempVec(temp[0], temp[1], temp[2]);
                rt->set_eye(tempVec);
                break;
            }
            case 'V':
            {
                vector<float> temp = get_float(temp_string);
                vec3 tempVec(temp[0], temp[1], temp[2]);
                rt->set_view_direction(tempVec);
                break;
            }
            case 'F':
            {
                vector<float> temp = get_float(temp_string);
                rt->set_fov(temp[0]);
                break;
            }
            case 'R':
            {
                vector<int> temp = get_int(temp_string);
                rt->set_resolution(temp[0], temp[1]);
                break;
            }
            case 'S':
            {
                vector<float> temp = get_float(temp_string);
                rt->create_sphere(temp[0], temp[1], temp[2], temp[3]);
                break;
            }
            case 'T':
            {
                vector<float> temp = get_float(temp_string);
                rt->create_triangle(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8]);
                break;
            }
            case 'L':
            {
                vector<float> temp = get_float(temp_string);
                rt->create_light(temp[0], temp[1], temp[2]);
                break;
            }
            case 'M':
            {
                rt->create_mesh();
                vector<float> temp = get_float(temp_string);
                rt->resigter_material(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], temp[9]);
                break;
            }
            default:
            {
                //wrong input
                break;
            }
        }
    }

    rt->start_trace();
    rt->output_file(argv[2]);
    delete rt;
}