#include <sphere.h>

//void

void Sphere::Set_on_scene(Scene &scene) {
    auto& pixels = scene.Get_canvas().pixels;

    for (size_t y = 0; y < scene.Get_canvas().Height(); ++y) {
        for (size_t x = 0; x < scene.Get_canvas().Width(); ++x) {
            auto ray_to_pixel = scene.Ray_to_pixel_from_camera(x, y);
            double dist;

            pixels[y][x] = Run_ray(scene.Get_camera_pos(), ray_to_pixel, pixels[y][x], dist);
        }
    }
}

Color Sphere::Run_ray(const Vector3<double> &origin, const Vector3<double> &ray, const Color& bg_color, double& dist) {
    if (Ray_intersect(origin, ray, dist)) {
        return color;
    }

    return bg_color;
}

//void Sphere

bool Sphere::Ray_intersect(const Vector3<double> &origin, const Vector3<double>& ray, double& dist_to_sphere) const {
    Vector3<double> line_by_ray = ray - origin;
    Vector3<double> line_to_center = center - origin;

    double distance_from_center_to_ray = line_by_ray.parallelogram_area(line_to_center) / line_by_ray.len();

    if (distance_from_center_to_ray < radius) {
        dist_to_sphere = sqrt(pow(line_to_center.len(), 2) + pow(distance_from_center_to_ray, 2));
    }
    else {
        distance_from_center_to_ray = std::numeric_limits<double>::max();
    }

    return distance_from_center_to_ray < radius;
}

void Sphere::Set_spheres_on_scene(Scene &scene, std::vector<Sphere> &spheres) {
    auto& pixels = scene.Get_canvas().pixels;

    for (size_t y = 0; y < scene.Get_canvas().Height(); ++y) {
        for (size_t x = 0; x < scene.Get_canvas().Width(); ++x) {
            auto ray_to_pixel = scene.Ray_to_pixel_from_camera(x, y);
            double min_dist = std::numeric_limits<double>::max();
            size_t min_dist_sphere_num = 0;

            for (size_t sphere_num = 0; sphere_num < spheres.size(); ++sphere_num) {
                double dist = 0;

                if (spheres[sphere_num].Ray_intersect(scene.Get_camera_pos(), ray_to_pixel, dist) && dist < min_dist) {
                    min_dist = dist;
                    min_dist_sphere_num = sphere_num;
                }
            }

            pixels[y][x] = spheres[min_dist_sphere_num].Run_ray(scene.Get_camera_pos(), ray_to_pixel, pixels[y][x], min_dist);
        }
    }
}
