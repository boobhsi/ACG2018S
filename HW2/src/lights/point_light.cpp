#include <lights/point_light.h>

PointLight::PointLight(vec4 pos, vec3 color) : Light(pos, color) {
    
}

PointLight::~PointLight() {

}

float PointLight::get_illuminance() {
    //TODO: return illuminance;
}