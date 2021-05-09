#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <canvas.h>

struct Light {
    Light(const Vector3<double>& position, double intensity);

    Vector3<double> position;
    double intensity;
};

class Scene {
public:
    void Set_canvas(size_t height, size_t width, const Color& bg_color);

    void Set_camera(const Vector3<double>& camera_pos, double fov);

    void Set_lights(const std::vector<Light>& lights);

    void Set_background_pic(const char* filepath, int desired_channels = 0);

    std::vector<std::vector<Color>>& Get_background_pic();

    const std::vector<Light>& Get_lights();

    Canvas& Get_canvas();

    const Vector3<double>& Get_camera_pos();

    // (x, y) - pixel pos from left up corner
    Vector3<double> Ray_to_pixel_from_camera(size_t x, size_t y);

    int Save_canvas_to_png(const char* filepath);

    Color Background_pixel(const Vector3<double>& ray_to_pixel) const;

private:
    Canvas _canvas;
    std::vector<std::vector<Color>> _background_pic;

    double _fov = 0; // vertical, in radians
    Vector3<double> _camera_pos;

    std::vector<Light> _lights;
};

#endif //RAYTRACING_SCENE_H
