#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include <geometry.h>
#include <scene.h>
#include <material.h>

class Sphere {
public:
    Sphere(const Vector3<double>& center, size_t radius, const Material& material):
        center(center.x, -center.y, center.z),
        radius(radius),
        material(material) {}

//    void Set_on_scene(Scene& scene);

    static void Set_spheres_on_scene(Scene& scene, std::vector<Sphere>& spheres);

private:
    bool Ray_intersect(const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere, Vector3<double>& normal) const;

    bool Ray_intersect(const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere) const;

    static bool Scene_intersect(std::vector<Sphere>& spheres, const Vector3<double>& origin, const Vector3<double>& ray_to_pixel, double& min_dist, size_t& min_dist_sphere_num, Vector3<double>& normal, Material& intersect_material);

    Color Run_ray(const Vector3<double>& origin, const Vector3<double>& ray, std::vector<Sphere> &spheres, const Scene& scene, const std::vector<Light>& lights, size_t depth = 0);

private:
    Vector3<double> center;
    size_t radius;
    Material material;
};

#endif //RAYTRACING_SPHERE_H
