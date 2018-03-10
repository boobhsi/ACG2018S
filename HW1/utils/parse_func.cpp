#ifndef PARSE_FUNC_H
#define PARSE_FUNC_H

#include <string>
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

vector<int> get_int(string input) {
    vector<int> answer;
    while(1){
        size_t space_pos = input.find(' ');
        string extracted = input.substr(0, space_pos);
        answer.push_back(stoi(extracted));
        if(space_pos == string::npos) {
            break;
        }
        input = input.substr(space_pos+1);
    }
    return answer;
}

vector<float> get_float(string input) {
    vector<float> answer;
    while(1){
        size_t c_pos = input.find_first_not_of(' ');

#ifdef DEBUG
        cout << input[c_pos] << 'c' << endl;
#endif

        input = input.substr(c_pos);
        size_t space_pos = input.find_first_of(' ');
        string extracted = input.substr(0, space_pos);
        
#ifdef DEBUG
        cout << extracted << 'a' << endl;
#endif

        answer.push_back(stof(extracted));
        if(space_pos == string::npos) {
            break;
        }
        input = input.substr(space_pos + 1);

#ifdef DEBUG
        cout << input << 'b' << endl;
#endif

    }
    return answer;
}

#endif