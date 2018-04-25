#include <lights/light.h>

Light::Light(vec4 pos, vec3 col, float pow) : position(pos), color(col), power(pow) {

}

Light::~Light() {

}

vec4 Light::get_position() {
    return position_t;
}

vec3 Light::get_color() {
    return color;
}

void Light::updateTransform(const mat4& transform_matrix) {
    mat4 temp = transform_matrix;
    position_t = position * temp;
}