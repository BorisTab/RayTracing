#include <cuda_ray_tracing.h>

__global__ void Cuda::Cuda_canvas_intersect(uchar3* d_canvas, Scene& scene, Sphere* spheres, size_t spheres_size, double double_max, Light* lights, size_t lights_size) {
    size_t y = blockDim.y * blockIdx.y + threadIdx.y;
    size_t x = blockDim.x * blockIdx.x + threadIdx.x;
    if (x >= scene._canvas._width || y >= scene._canvas._height) {
        return;
    }

    printf("%lud %lu\n", x, y);

    size_t pixel_num = y * scene._canvas._width + x;

    auto ray_to_pixel = SceneF::Ray_to_pixel_from_camera(scene, x, y);
    double min_dist = double_max;
    size_t min_dist_sphere_num = 0;
    Vector3<double> normal;
    Material sphere_material;

    SphereF::Scene_intersect(spheres, spheres_size, scene._camera_pos, ray_to_pixel, min_dist, min_dist_sphere_num, normal, sphere_material);
    min_dist = double_max;

    printf("%lu", pixel_num);
//    copy_simple_color(SphereF::Run_ray(spheres[min_dist_sphere_num], scene._camera_pos, Vec3::normalize(ray_to_pixel), spheres, spheres_size, scene, lights, lights_size), d_canvas[pixel_num]);
//    d_canvas[pixel_num] = SphereF::Run_ray(spheres[min_dist_sphere_num], scene._camera_pos, Vec3::normalize(ray_to_pixel), spheres, spheres_size, scene, lights, lights_size);
}