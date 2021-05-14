#include <material.h>

void MaterialF::MaterialConstructDefault(Material& material) {
    material.albedo[0] = 0;
    material.albedo[1] = 1;
    material.albedo[2] = 0;
    material.specular_power = 1;
    material.reflectivity = 0;
    material.refract_index = 1;
    material.refractivity = 0;
}

void MaterialF::MaterialConstruct(Material& material, const SimpleColor& color, const double albedo[3], int specular_power, double reflectivity, double refract_index, double refractivity) {
    material.diffuse_color = color;
    material.albedo[0] = albedo[0];
    material.albedo[1] = albedo[1];
    material.albedo[2] = albedo[2];
    material.specular_power = specular_power;
    material.reflectivity = reflectivity;
    material.refract_index = refract_index;
    material.refractivity = refractivity;
}
