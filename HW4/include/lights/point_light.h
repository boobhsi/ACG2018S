#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <include.h>
#include <lights/light.h>
#include <utils/algebra3.h>

class PointLight : public Light
{
public:
    PointLight(vec4 pos, vec3 color, float pow);
    ~PointLight();
    float get_illuminance(vec4 pos);
private:
};

#endif