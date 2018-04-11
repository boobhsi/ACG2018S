#include <lights/point_light.h>

PointLight::PointLight(vec4 pos, vec3 color, float pow) : Light(pos, color, pow) {
    
}

PointLight::~PointLight() {

}

float PointLight::get_illuminance(vec4 pos) {
    float distance = (pos - position_t).length2();
    //assert(distance > 0.001f);
    if(distance < 0.00001f) return power;
    return power / distance;
    //float i = power;
}