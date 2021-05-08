#include <sphere.h>

#include <iostream>

//void

//void Sphere::Set_on_scene(Scene &scene) {
//    auto& pixels = scene.Get_canvas().pixels;
//
//    for (size_t y = 0; y < scene.Get_canvas().Height(); ++y) {
//        for (size_t x = 0; x < scene.Get_canvas().Width(); ++x) {
//            auto ray_to_pixel = scene.Ray_to_pixel_from_camera(x, y);
//            double dist;
//            std::vector<Sphere> spheres;
//
//            pixels[y][x] = Run_ray(scene.Get_camera_pos(), ray_to_pixel, spheres, pixels[y][x], dist, scene.Get_lights());
//        }
//    }
//}

Color Sphere::Run_ray(const Vector3<double> &origin, const Vector3<double> &ray, std::vector<Sphere> &spheres, const Color& bg_color, const std::vector<Light>& lights, size_t depth) {
     Vector3<double> normal(0, 0, 0);
    double diffuse_light_intensity = 0;
    double specular_light_intensity = 0;
    size_t min_dist_sphere_num = 0;
    double dist = std::numeric_limits<double>::max();
    Material intersect_material;

    if (depth <= 4 && Scene_intersect(spheres, origin, ray, dist, min_dist_sphere_num, normal, intersect_material)) {
        auto dir_from_camera_to_point = ray.normalized() * dist;
        auto dir_to_point = origin + dir_from_camera_to_point;

        auto reflected_ray_dir = ray.normalized().reflect_with(normal.normalized());
        auto reflected_ray_origin = reflected_ray_dir * normal < 0 ?  dir_to_point - normal * 1e-3 : dir_to_point + normal * 1e-3;

        Color reflected_color = Run_ray(reflected_ray_origin, reflected_ray_dir, spheres, bg_color, lights, depth + 1);

        for (auto& light: lights) {
            auto light_dir_from_point_to_light = (light.position - dir_to_point);
            double light_dist = light_dir_from_point_to_light.len();
            light_dir_from_point_to_light = light_dir_from_point_to_light.normalized();

            auto shadow_origin = light_dir_from_point_to_light * normal < 0 ?  dir_to_point - normal * 1e-3 : dir_to_point + normal * 1e-3;

            double min_shadow_dist = std::numeric_limits<double>::max();
            Vector3<double> shadow_normal;
//            size_t min_dist_sphere_num = 100;
            Material shad_mat;
            if (Scene_intersect(spheres, shadow_origin, light_dir_from_point_to_light, min_shadow_dist, min_dist_sphere_num, shadow_normal, shad_mat) && min_shadow_dist < light_dist) {
                continue;
            }

            diffuse_light_intensity += light.intensity * std::max(0., light_dir_from_point_to_light * normal.normalized());
            specular_light_intensity += light.intensity * pow(std::max(0., light_dir_from_point_to_light.reflect_with(normal) * ray), intersect_material.specular_power);
        }

        return intersect_material.diffuse_color * intersect_material.albedo.x +
               intersect_material.diffuse_color * diffuse_light_intensity * intersect_material.albedo.y +
               Color(255, 255, 255) * specular_light_intensity * intersect_material.albedo.z +
               reflected_color * intersect_material.reflectivity;
    }

    return bg_color;
}

//void Sphere

bool Sphere::Ray_intersect(const Vector3<double> &origin, const Vector3<double>& ray, double& dist_to_sphere, Vector3<double>& normal) const {
    Vector3<double> line_to_center = center - origin;

    double distance_from_center_to_ray = ray.parallelogram_area(line_to_center) / ray.len();

    if (distance_from_center_to_ray <= radius) {
        if (line_to_center.cos(ray) < 0) {
            return false;
        }

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
    Vector3<double> line_to_center = center - origin;

    double distance_from_center_to_ray = ray.parallelogram_area(line_to_center) / ray.len();

    if (distance_from_center_to_ray <= radius) {
        if (line_to_center.cos(ray) < 0) {
            return false;
        }
        dist_to_sphere = line_to_center.len() * line_to_center.cos(ray) -
                         sqrt(radius * radius - distance_from_center_to_ray * distance_from_center_to_ray);
    }
    else {
        distance_from_center_to_ray = std::numeric_limits<double>::max();
    }

    return distance_from_center_to_ray < radius;
}

bool Sphere::Scene_intersect(std::vector<Sphere>& spheres, const Vector3<double>& origin, const Vector3<double>& ray_to_pixel, double& min_dist, size_t& min_dist_sphere_num, Vector3<double>& normal, Material& intersect_material) {
    for (size_t sphere_num = 0; sphere_num < spheres.size(); ++sphere_num) {
        double dist = 0;
        Vector3<double> cur_normal;

        if (spheres[sphere_num].Ray_intersect(origin, ray_to_pixel, dist, cur_normal) && dist < min_dist) {
            min_dist = dist;
            min_dist_sphere_num = sphere_num;
            normal = cur_normal;
            intersect_material = spheres[sphere_num].material;
        }
    }

    return min_dist < std::numeric_limits<double>::max();
}

void Sphere::Set_spheres_on_scene(Scene &scene, std::vector<Sphere> &spheres) {
    auto& pixels = scene.Get_canvas().pixels;

    for (size_t y = 0; y < scene.Get_canvas().Height(); ++y) {
        for (size_t x = 0; x < scene.Get_canvas().Width(); ++x) {
//            if (y == 425 && x == 1170) {
//                printf("a\n");
//            }

            auto ray_to_pixel = scene.Ray_to_pixel_from_camera(x, y);
            double min_dist = std::numeric_limits<double>::max();
            size_t min_dist_sphere_num = 0;
            Vector3<double> normal;
            Material sphere_material;

            Scene_intersect(spheres, scene.Get_camera_pos(), ray_to_pixel, min_dist, min_dist_sphere_num, normal, sphere_material);

            min_dist = std::numeric_limits<double>::max();
            pixels[y][x] = spheres[min_dist_sphere_num].Run_ray(scene.Get_camera_pos(), ray_to_pixel, spheres, pixels[y][x], scene.Get_lights());

            if (y == 425 && x == 1170) {
                pixels[y][x] = {0, 0, 0};
            }
        }
    }
}
