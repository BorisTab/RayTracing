#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include <geometry.h>
#include <scene.h>

class Sphere {
public:
    Sphere(const Vector3<double>& center, size_t radius, const Color& color):
        center(center.x, -center.y, center.z),
        radius(radius),
        color(color) {}

    void Set_on_scene(Scene& scene);

    static void Set_spheres_on_scene(Scene& scene, std::vector<Sphere>& spheres);

private:
    bool Ray_intersect(const Vector3<double>& origin, const Vector3<double>& ray, double& dist_to_sphere) const;

    Color Run_ray(const Vector3<double>& origin, const Vector3<double>& ray, const Color& bg_color, double& dist);

private:
    Vector3<double> center;
    size_t radius;
    Color color;
};

#endif //RAYTRACING_SPHERE_H
