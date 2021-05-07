#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include <geometry.h>
#include <scene.h>

//struct Material {
//    Material(const Color& color): diffuse_color(color) {}
//
//    Color diffuse_color;
//};

class Sphere {
public:
    Sphere(const Vector3<double>& center, size_t radius, const Color& color):
        center(center.x, -center.y, center.z),
        radius(radius),
        color(color) {}

    void Set_on_scene(Scene& scene);

    static void Set_spheres_on_scene(Scene& scene, std::vector<Sphere>& spheres);

private:
    bool Ray_intersect(const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere, Vector3<double>& normal) const;

    bool Ray_intersect(const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere) const;

    Color Run_ray(const Vector3<double>& origin, const Vector3<double>& ray, const Color& bg_color, double& dist, const std::vector<Light>& lights);

private:
    Vector3<double> center;
    size_t radius;
    Color color;
};

#endif //RAYTRACING_SPHERE_H
