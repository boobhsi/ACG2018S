#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <lights/light.h>
#include <utils/algebra3.h>

class PointLight : public Light
{
public:
    PointLight(vec4 pos, vec3 color);
    ~PointLight();
    float get_illuminance();
private:
};

#endif