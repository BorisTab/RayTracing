#include <sphere.h>

#include <iostream>
#include <limits>
#include <future>

Color Sphere::Run_ray(const Vector3<double> &origin, const Vector3<double> &ray, std::vector<Sphere> &spheres, const Scene& scene, const std::vector<Light>& lights, size_t depth) {
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
        auto refracted_ray_dir = Vector3<double>::refract(ray.normalized(), normal.normalized(), intersect_material.refract_index);

        auto reflected_ray_origin = reflected_ray_dir * normal < 0 ?  dir_to_point - normal * 1e-3 : dir_to_point + normal * 1e-3;
        auto refracted_ray_origin = refracted_ray_dir * normal < 0 ?  dir_to_point - normal * 1e-3 : dir_to_point + normal * 1e-3;

        Color reflected_color = Run_ray(reflected_ray_origin, reflected_ray_dir, spheres, scene, lights, depth + 1);
        Color refracted_color = Run_ray(refracted_ray_origin, refracted_ray_dir, spheres, scene, lights, depth + 1);

        for (auto& light: lights) {
            auto light_dir_from_point_to_light = (light.position - dir_to_point);
            double light_dist = light_dir_from_point_to_light.len();
            light_dir_from_point_to_light = light_dir_from_point_to_light.normalized();

            auto shadow_origin = light_dir_from_point_to_light * normal < 0 ?  dir_to_point - normal * 1e-3 : dir_to_point + normal * 1e-3;

            double min_shadow_dist = std::numeric_limits<double>::max();
            Vector3<double> shadow_normal;
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
               reflected_color * intersect_material.reflectivity +
               refracted_color * intersect_material.refractivity;
    }


    return scene.Background_pixel(ray);
}

//void Sphere

bool Sphere::Ray_intersect(const Vector3<double> &origin, const Vector3<double>& ray, double& dist_to_sphere, Vector3<double>& normal) const {
    Vector3<double> line_to_center = center - origin;
    double distance_from_center_to_ray = ray.parallelogram_area(line_to_center) / ray.len();
    double min_dist = 0;

    if (distance_from_center_to_ray <= radius) {
//        if (line_to_center.cos(ray) < 0) {
//            return false;
//        }

        min_dist = line_to_center.len() * line_to_center.cos(ray) -
                sqrt(radius * radius - distance_from_center_to_ray * distance_from_center_to_ray);
        if (min_dist < 0) {
            return false;
        }

        dist_to_sphere = min_dist;
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

    if (fabs(ray_to_pixel.y) > 1e-3)  {
        double plane_dist = -(origin.y-4)/ray_to_pixel.y; // the checkerboard plane has equation y = -4
        Vector3<double> pt = origin + ray_to_pixel * plane_dist;
        if (plane_dist > 0 && fabs(pt.x) < 10 && pt.z > 10 && pt.z < 30 && plane_dist < min_dist) {
            min_dist = plane_dist;
            normal = Vector3<double>(0,-1,0);
            intersect_material.albedo.x = 0.3;
            intersect_material.albedo.y = 1;

            intersect_material.diffuse_color = (int(.5 * pt.x + 1000) + int(.5 * pt.z)) & 1 ? Color(255, 255, 255) : Color(255, 179, 76);
        }
    }

    return min_dist < 1000000;
}

void Sphere::Set_spheres_on_scene(Scene &scene, std::vector<Sphere> &spheres) {
    auto start = std::clock();

    auto& pixels = scene.Get_canvas().pixels;
    int thread_num = 1;
    size_t one_chunk = floor(scene.Get_canvas().Height() / 12) * scene.Get_canvas().Width();

    std::vector<std::future<void>> work_lines(thread_num);

    for (int cur_thread = 0; cur_thread < thread_num; ++cur_thread) {
        work_lines[cur_thread] = std::async(std::launch::async, [&scene, &spheres, &pixels, cur_thread, one_chunk, thread_num] () mutable {
            size_t chunk_end = (cur_thread == thread_num - 1) ? scene.Get_canvas().Height() * scene.Get_canvas().Width() : (cur_thread + 1)  * one_chunk;

            for (size_t pixel_num = cur_thread * one_chunk; pixel_num < chunk_end; ++pixel_num) {
                auto ray_to_pixel = scene.Ray_to_pixel_from_camera(pixel_num % scene.Get_canvas().Width(), pixel_num / scene.Get_canvas().Width());
                double min_dist = std::numeric_limits<double>::max();
                size_t min_dist_sphere_num = 0;
                Vector3<double> normal;
                Material sphere_material;

                Scene_intersect(spheres, scene.Get_camera_pos(), ray_to_pixel, min_dist, min_dist_sphere_num, normal, sphere_material);
                ray_to_pixel = ray_to_pixel.normalized();
                min_dist = std::numeric_limits<double>::max();

                pixels[pixel_num] = spheres[min_dist_sphere_num].Run_ray(scene.Get_camera_pos(), ray_to_pixel, spheres, scene, scene.Get_lights());
            }
        });
    }

    for(auto& work_line: work_lines) {
        work_line.wait();
    }

    auto time = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    printf("%g\n", time);

    // for (size_t pixel_num = 0; pixel_num < scene.Get_canvas().Height() * scene.Get_canvas().Width(); ++pixel_num) {
    //     auto ray_to_pixel = scene.Ray_to_pixel_from_camera(pixel_num % scene.Get_canvas().Width(), pixel_num / scene.Get_canvas().Width());
    //     double min_dist = std::numeric_limits<double>::max();
    //     size_t min_dist_sphere_num = 0;
    //     Vector3<double> normal;
    //     Material sphere_material;

    //     Scene_intersect(spheres, scene.Get_camera_pos(), ray_to_pixel, min_dist, min_dist_sphere_num, normal, sphere_material);
    //     ray_to_pixel = ray_to_pixel.normalized();
    //     min_dist = std::numeric_limits<double>::max();

    //     pixels[pixel_num] = spheres[min_dist_sphere_num].Run_ray(scene.Get_camera_pos(), ray_to_pixel, spheres, scene, scene.Get_lights());
    // }
}
