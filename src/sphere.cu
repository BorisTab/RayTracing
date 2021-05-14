#include <sphere.h>
#include <cuda_error_checker.h>
#include <cuda_ray_tracing.h>

#include <iostream>
#include <limits>
#include <future>

#define DIST_MAX 1000000000000000000000000000.

__device__ uchar3 SphereF::Run_ray(Sphere& sphere, const Vector3<double> &origin, const Vector3<double> &ray, Sphere* spheres, size_t spheres_size, const Scene& scene, const Light* lights, size_t lights_size, size_t depth) {
    Vector3<double> normal;
    double diffuse_light_intensity = 0;
    double specular_light_intensity = 0;
    size_t min_dist_sphere_num = 0;
    double dist = DIST_MAX;
    Material intersect_material;

    if (depth <= 4 && Scene_intersect(spheres, spheres_size, origin, ray, dist, min_dist_sphere_num, normal, intersect_material)) {
        auto dir_from_camera_to_point = Vec3::normalize(ray) * dist;
        auto dir_to_point = origin + dir_from_camera_to_point;

        auto reflected_ray_dir = Vec3::reflect(Vec3::normalize(ray), Vec3::normalize(normal));
        auto refracted_ray_dir = Vec3::refract(Vec3::normalize(ray), Vec3::normalize(normal), intersect_material.refract_index);

        auto reflected_ray_origin = reflected_ray_dir * normal < 0 ?  dir_to_point - normal * 1e-3 : dir_to_point + normal * 1e-3;
        auto refracted_ray_origin = refracted_ray_dir * normal < 0 ?  dir_to_point - normal * 1e-3 : dir_to_point + normal * 1e-3;

        uchar3 reflected_color = Run_ray(sphere, reflected_ray_origin, reflected_ray_dir, spheres, spheres_size, scene, lights, lights_size, depth + 1);
        uchar3 refracted_color = Run_ray(sphere, refracted_ray_origin, refracted_ray_dir, spheres, spheres_size, scene, lights, lights_size, depth + 1);

        for (int light_num = 0; light_num < lights_size; ++light_num) {
            auto light_dir_from_point_to_light = (lights[light_num].position - dir_to_point);
            double light_dist = Vec3::len(light_dir_from_point_to_light);
            Vec3::normalize_self(light_dir_from_point_to_light);

            auto shadow_origin = light_dir_from_point_to_light * normal < 0 ?  dir_to_point - normal * 1e-3 : dir_to_point + normal * 1e-3;

            double min_shadow_dist = DIST_MAX;
            Vector3<double> shadow_normal;
            Material shad_mat;

            if (Scene_intersect(spheres, spheres_size, shadow_origin, light_dir_from_point_to_light, min_shadow_dist, min_dist_sphere_num, shadow_normal, shad_mat) && min_shadow_dist < light_dist) {
                continue;
            }

            diffuse_light_intensity += lights[light_num].intensity * max(0., light_dir_from_point_to_light * Vec3::normalize(normal));
            specular_light_intensity += lights[light_num].intensity * pow(max(0., Vec3::reflect(light_dir_from_point_to_light, normal) * ray), intersect_material.specular_power);
        }

        uchar3 white_color = {255, 255, 255};
//        SimpleColor out_color = intersect_material.diffuse_color;

        return white_color;
//        return out_color * intersect_material.albedo[0] +
//               out_color * diffuse_light_intensity * intersect_material.albedo[1] +
//               white_color * specular_light_intensity * intersect_material.albedo[2] +
//               reflected_color * intersect_material.reflectivity +
//               refracted_color * intersect_material.refractivity;
    }

    uchar3 white_color = {255, 255, 255};
    return white_color;//SceneF::Background_pixel(scene, ray);
}

//void Sphere

__device__ bool SphereF::Ray_intersect(Sphere& sphere, const Vector3<double> &origin, const Vector3<double>& ray, double& dist_to_sphere, Vector3<double>& normal) {
    Vector3<double> line_to_center = sphere.center - origin;
    double distance_from_center_to_ray = Vec3::parallelogram_area(ray, line_to_center) / Vec3::len(ray);
    double min_dist = 0;

    if (distance_from_center_to_ray <= sphere.radius) {
//        if (line_to_center.cos(ray) < 0) {
//            return false;
//        }

        min_dist = Vec3::len(line_to_center) * (Vec3::normalize(line_to_center) * Vec3::normalize(ray)) -
                sqrt(sphere.radius * sphere.radius - distance_from_center_to_ray * distance_from_center_to_ray);
        if (min_dist < 0) {
            return false;
        }

        dist_to_sphere = min_dist;
        Vec3::copy_vec(Vec3::normalize(ray) * dist_to_sphere - line_to_center, normal);
    }
    else {
        distance_from_center_to_ray = DIST_MAX;
    }

    return distance_from_center_to_ray < sphere.radius;
}

//bool SphereF::Ray_intersect(Sphere& sphere, const Vector3<double> &origin, const Vector3<double>& ray, double& dist_to_sphere) {
//    Vector3<double> line_to_center = sphere.center - origin;
//
//    double distance_from_center_to_ray = ray.parallelogram_area(line_to_center) / ray.len();
//
//    if (distance_from_center_to_ray <= sphere.radius) {
//        if (line_to_center.cos(ray) < 0) {
//            return false;
//        }
//        dist_to_sphere = line_to_center.len() * line_to_center.cos(ray) -
//                         sqrt(sphere.radius * sphere.radius - distance_from_center_to_ray * distance_from_center_to_ray);
//    }
//    else {
//        distance_from_center_to_ray = std::numeric_limits<double>::max();
//    }
//
//    return distance_from_center_to_ray < sphere.radius;
//}

__device__ bool SphereF::Scene_intersect(Sphere* spheres, size_t spheres_size, const Vector3<double>& origin, const Vector3<double>& ray_to_pixel, double& min_dist, size_t& min_dist_sphere_num, Vector3<double>& normal, Material& intersect_material) {
    for (size_t sphere_num = 0; sphere_num < spheres_size; ++sphere_num) {
        double dist = 0;
        Vector3<double> cur_normal;

        if (Ray_intersect(spheres[sphere_num], origin, ray_to_pixel, dist, cur_normal) && dist < min_dist) {
            min_dist = dist;
            min_dist_sphere_num = sphere_num;

            Vec3::copy_vec(cur_normal, normal);
            intersect_material = spheres[sphere_num].material;
        }
    }

    if (fabs(ray_to_pixel.y) > 1e-3)  {
        double plane_dist = -(origin.y-4)/ray_to_pixel.y; // the checkerboard plane has equation y = -4
        Vector3<double> pt = origin + ray_to_pixel * plane_dist;
        if (plane_dist > 0 && fabs(pt.x) < 10 && pt.z > 10 && pt.z < 30 && plane_dist < min_dist) {
            min_dist = plane_dist;

            Vec3::set_val(normal, 0., -1., 0.);

            intersect_material.albedo[0] = 0.3;
            intersect_material.albedo[1] = 1;

            SimpleColor white = {255, 255, 255};
            SimpleColor yellow = {255, 179, 76};

            intersect_material.diffuse_color = (int(.5 * pt.x + 1000) + int(.5 * pt.z)) & 1 ? white : yellow;
        }
    }

    return min_dist < 1000000;
}

void SphereF::Set_spheres_on_scene(Scene &scene, std::vector<Sphere> &spheres) {
    auto start = std::clock();

    checkCudaErrors(cudaSetDevice(0));

    auto* pixels = scene._canvas.pixels;
    size_t num_of_pixels = scene._canvas._height * scene._canvas._width;

    uchar3* d_canvas = nullptr;
    checkCudaErrors(cudaMalloc(&d_canvas, sizeof(uchar3) * num_of_pixels));
    checkCudaErrors(cudaMemcpy(d_canvas, pixels, sizeof(uchar3) * num_of_pixels, cudaMemcpyHostToDevice));

    size_t threadNum = 32;
    dim3 blockSize(threadNum, 1, 1);
    dim3 gridSize(scene._canvas._width / threadNum + 1, scene._canvas._height, 1);

    Cuda::Cuda_canvas_intersect<<<gridSize, blockSize>>>(d_canvas, scene, &spheres.front(), spheres.size(), DIST_MAX, &scene._lights.front(), scene._lights.size());
    checkCudaErrors(cudaGetLastError());

    cudaDeviceSynchronize();
    checkCudaErrors(cudaGetLastError());

//    checkCudaErrors(cudaMemcpy(pixels, d_canvas, sizeof(SimpleColor) * num_of_pixels, cudaMemcpyDeviceToHost));
    checkCudaErrors(cudaFree(d_canvas));

    auto time = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    printf("%g\n", time);
}
