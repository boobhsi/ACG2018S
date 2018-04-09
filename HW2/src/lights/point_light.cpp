#include <lights/point_light.h>
#include <cassert>

PointLight::PointLight(vec4 pos, vec3 color, float pow) : Light(pos, color, pow) {
    
}

PointLight::~PointLight() {

}

float PointLight::get_illuminance(vec4 pos) {
    float distance = (pos - position_t).length();
    assert(distance > 0.001f);
    float i = power / pow(distance, 2.0);
    return i;
}