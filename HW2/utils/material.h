#ifndef MATERIAL_H
#define MATERIAL_H

#include "algebra3.h"

struct Material {
    vec3 color;
    vec3 surface_properties;
    float specular;
    vec2 scatter_ratio;
    float density;
};

#endif