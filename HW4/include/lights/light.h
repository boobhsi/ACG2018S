#ifndef LIGHT_H
#define LIGHT_H

#include <include.h>
#include <utils/algebra3.h>

class Light 
{
public:
    Light(vec4 pos, vec3 col, float pow);
    virtual ~Light();
    virtual float get_illuminance(vec4 position) = 0;
    vec4 get_position();
    vec3 get_color();
    void updateTransform(const mat4& transform_matrix);
protected:
    vec4 position;
    vec4 position_t;
    vec3 color;
    float power;
};

#endif