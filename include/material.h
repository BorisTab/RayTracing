#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include <geometry.h>

struct Material {
//    Material():
//            diffuse_color(),
//            albedo({0, 1, 0}),
//            specular_power(1),
//            reflectivity(0),
//            refract_index(1),
//            refractivity(0) {}
//
//    Material(const Color& color, const Vector3<double>& albedo, int specular_power, double reflectivity, double refract_index, double refractivity):
//            diffuse_color(color),
//            albedo(albedo),
//            specular_power(specular_power),
//            reflectivity(reflectivity),
//            refract_index(refract_index),
//            refractivity(refractivity) {}

    SimpleColor diffuse_color;

    // albedo:
    // x - ambient_coef
    // y - diffuse_coef
    // z - specular_coef
    double albedo[3];
    int specular_power;
    double reflectivity;
    double refract_index;
    double refractivity;
};

namespace MaterialF {
    void MaterialConstructDefault(Material& material);

    void MaterialConstruct(Material& material, const SimpleColor& color, const double albedo[3], int specular_power, double reflectivity, double refract_index, double refractivity);
}

#endif //RAYTRACING_MATERIAL_H
