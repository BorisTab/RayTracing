#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <canvas.h>

class Scene {
public:
    void Set_canvas(size_t height, size_t width, const Color& bg_color);

    void Set_camera(const Vector3<double>& camera_pos, double fov);

    Canvas& Get_canvas();

    const Vector3<double>& Get_camera_pos();

    // (x, y) - pixel pos from left up corner
    Vector3<double> Ray_to_pixel_from_camera(size_t x, size_t y);

    int Save_canvas_to_png(const char* filepath);

private:
    Canvas _canvas;
    double _fov = 0; // vertical, in radians
    Vector3<double> _camera_pos;
};

#endif //RAYTRACING_SCENE_H
