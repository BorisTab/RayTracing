#include <scene.h>

Light::Light(const Vector3<double> &position, double intensity):
    position(position.x, -position.y, position.z),
    intensity(intensity) {}

void Scene::Set_canvas(size_t height, size_t width, const Color &bg_color) {
    _canvas.Setup(height, width, bg_color);
}

void Scene::Set_camera(const Vector3<double> &camera_pos, double fov) {
    _camera_pos = camera_pos;
    _fov = fov;
}

void Scene::Set_lights(const std::vector<Light> &lights) {
    _lights = lights;
}

const std::vector<Light>& Scene::Get_lights() {
    return _lights;
}

Vector3<double> Scene::Ray_to_pixel_from_camera(size_t x, size_t y) {
    double z_pos = _camera_pos.z + 1;

    double canvas_height = 2 * z_pos * std::tan(_fov / 2.);
    double canvas_width = canvas_height * _canvas.Width() / (_canvas.Height() * 1.);

    double x_pos = ((x + 0.5) / (_canvas.Width()  * 1.) - 0.5) * canvas_width;
    double y_pos = ((y + 0.5) / (_canvas.Height() * 1.) - 0.5) * canvas_height;

    return Vector3(x_pos, y_pos, z_pos);
}

Canvas& Scene::Get_canvas() {
    return _canvas;
}

const Vector3<double> & Scene::Get_camera_pos() {
    return _camera_pos;
}

int Scene::Save_canvas_to_png(const char* filepath) {
    return _canvas.Save_to_png(filepath);
}
