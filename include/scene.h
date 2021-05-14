#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <canvas.h>

struct Light {
    __device__ Light(const Vector3<double>& position, double intensity);

    Vector3<double> position;
    double intensity;
};

struct Scene {
//    void Set_canvas(size_t height, size_t width, const Color& bg_color);
//
//    void Set_camera(const Vector3<double>& camera_pos, double fov);
//
//    void Set_lights(const std::vector<Light>& lights);
//
//    void Set_background_pic(const char* filepath, int desired_channels = 0);
//
//    std::vector<std::vector<Color>>& Get_background_pic();
//
//    const std::vector<Light>& Get_lights();
//
//    Canvas& Get_canvas();
//
//    const Vector3<double>& Get_camera_pos();
//
//    // (x, y) - pixel pos from left up corner
//    Vector3<double> Ray_to_pixel_from_camera(size_t x, size_t y);
//
//    int Save_canvas_to_png(const char* filepath);
//
//    Color Background_pixel(const Vector3<double>& ray_to_pixel) const;

    Canvas _canvas;
    std::vector<std::vector<Color>> _background_pic;
    size_t _background_pic_height;
    size_t _background_pic_width;

    double _fov = 0; // vertical, in radians
    Vector3<double> _camera_pos;

    std::vector<Light> _lights;
};

namespace SceneF {
    void Set_canvas(Scene& scene, size_t height, size_t width, const Color& bg_color);

    void Set_camera(Scene& scene, const Vector3<double>& camera_pos, double fov);

    void Set_lights(Scene& scene, const std::vector<Light>& lights);

    void Set_background_pic(Scene& scene, const char* filepath, int desired_channels = 0);

    std::vector<std::vector<Color>>& Get_background_pic(Scene& scene);

// (x, y) - pixel pos from left up corner
    __device__ Vector3<double> Ray_to_pixel_from_camera(Scene& scene, size_t x, size_t y);

    int Save_canvas_to_png(Scene& scene, const char* filepath);

    __device__ SimpleColor Background_pixel(const Scene& scene, const Vector3<double>& ray_to_pixel);
}

#endif //RAYTRACING_SCENE_H
