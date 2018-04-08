#ifndef LIGHT_H
#define LIGHT_H

#include <utils/algebra3.h>

class Light 
{
public:
    Light(vec4 pos, vec3 col);
    ~Light();
    virtual float get_illuminance() = 0;
    vec4 get_position();
    vec3 get_color();
    void updateTransform(const mat4& transform_matrix);
private:
    vec4 position;
    vec4 position_t;
    vec3 color;
};

#endif