#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include <geometry.h>
#include <scene.h>
#include <material.h>

struct Sphere {
//    Sphere(const Vector3<double>& center, size_t radius, const Material& material):
//        center(center.x, -center.y, center.z),
//        radius(radius),
//        material(material) {}
//
////    void Set_on_scene(Scene& scene);
//
//    static void Set_spheres_on_scene(Scene& scene, std::vector<Sphere>& spheres);
//
//    bool Ray_intersect(const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere, Vector3<double>& normal) const;
//
//    bool Ray_intersect(const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere) const;
//
//    static bool Scene_intersect(std::vector<Sphere>& spheres, const Vector3<double>& origin, const Vector3<double>& ray_to_pixel, double& min_dist, size_t& min_dist_sphere_num, Vector3<double>& normal, Material& intersect_material);
//
//    Color Run_ray(const Vector3<double>& origin, const Vector3<double>& ray, std::vector<Sphere> &spheres, const Scene& scene, const std::vector<Light>& lights, size_t depth = 0);

    Vector3<double> center;
    size_t radius;
    Material material;
};

namespace SphereF {
    void SphereConstruct(Sphere& sphere, const Vector3<double>& center, size_t radius, const Material& material);
//            center(center.x, -center.y, center.z),
//            radius(radius),
//            material(material) {}

//    void Set_on_scene(Scene& scene);

    void Set_spheres_on_scene(Scene& scene, std::vector<Sphere>& spheres);

    __device__ bool Ray_intersect(Sphere& sphere, const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere, Vector3<double>& normal);

//    bool Ray_intersect(Sphere& sphere, const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere);

    __device__ bool Scene_intersect(Sphere* spheres, size_t spheres_size, const Vector3<double>& origin, const Vector3<double>& ray_to_pixel, double& min_dist, size_t& min_dist_sphere_num, Vector3<double>& normal, Material& intersect_material);

    __device__ SimpleColor Run_ray(Sphere& sphere, const Vector3<double>& origin, const Vector3<double>& ray, Sphere* spheres, size_t spheres_size, const Scene& scene, const Light* lights, size_t lights_size, size_t depth = 0);
}

#endif //RAYTRACING_SPHERE_H
