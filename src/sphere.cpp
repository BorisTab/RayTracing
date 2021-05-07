#include <sphere.h>

#include <iostream>

//void

void Sphere::Set_on_scene(Scene &scene) {
    auto& pixels = scene.Get_canvas().pixels;

    for (size_t y = 0; y < scene.Get_canvas().Height(); ++y) {
        for (size_t x = 0; x < scene.Get_canvas().Width(); ++x) {
            auto ray_to_pixel = scene.Ray_to_pixel_from_camera(x, y);
            double dist;

            pixels[y][x] = Run_ray(scene.Get_camera_pos(), ray_to_pixel, pixels[y][x], dist, scene.Get_lights());
        }
    }
}

Color Sphere::Run_ray(const Vector3<double> &origin, const Vector3<double> &ray, const Color& bg_color, double& dist, const std::vector<Light>& lights) {
    Vector3<double> normal(0, 0, 0);
    double diffuse_light_intensity = 0;

    if (Ray_intersect(origin, ray, dist, normal)) {
        auto dir_from_camera_to_point = (ray - origin).normalized() * dist;

        for (auto& light: lights) {
            auto light_dir_form_point_to_light = (light.position - origin - dir_from_camera_to_point).normalized();
            diffuse_light_intensity += light.intensity * std::max(0., light_dir_form_point_to_light * normal.normalized());
//            0.5 - light_dir_form_point_to_light.cos(normal.normalized()) / 2.
//            printf("%g", light_dir_form_point_to_light.cos(normal.normalized()));
//            printf("%g ", diffuse_light_intensity);

//            if (light_dir_form_point_to_light * normal.normalized() > 0) {
//                printf("%g\n", light_dir_form_point_to_light * normal.normalized());
//            }
        }

        return color * diffuse_light_intensity;
    }

    return bg_color;
}

//void Sphere

bool Sphere::Ray_intersect(const Vector3<double> &origin, const Vector3<double>& ray, double& dist_to_sphere, Vector3<double>& normal) const {
    Vector3<double> line_to_center = center - origin;

    double distance_from_center_to_ray = ray.parallelogram_area(line_to_center) / ray.len();

    if (distance_from_center_to_ray <= radius) {
        dist_to_sphere = line_to_center.len() * line_to_center.cos(ray) -
                sqrt(radius * radius - distance_from_center_to_ray * distance_from_center_to_ray);
        normal = ray.normalized() * dist_to_sphere - line_to_center;
    }
    else {
        distance_from_center_to_ray = std::numeric_limits<double>::max();
    }

    return distance_from_center_to_ray < radius;
}

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

            pixels[y][x] = spheres[min_dist_sphere_num].Run_ray(scene.Get_camera_pos(), ray_to_pixel, pixels[y][x], min_dist, scene.Get_lights());
        }
    }
}
