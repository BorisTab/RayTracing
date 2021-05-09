#include <scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

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

void Scene::Set_background_pic(const char* filepath, int desired_channels) {
    int width = 0;
    int height = 0;
    int channels_in_file = 0;
    unsigned char* buffer = stbi_load(filepath, &width, &height, &channels_in_file, desired_channels);

    printf("chnnels in file: %d, size: %d x %d\n", channels_in_file, width, height);

    _background_pic.resize(height);
    int pixel_pos_in_buffer = 0;
    for (int line_num = 0; line_num < height; ++line_num) {
        _background_pic[line_num].resize(width);

        for (int pixel_pos = 0; pixel_pos < width; ++pixel_pos) {
            pixel_pos_in_buffer += 3;
            _background_pic[line_num][pixel_pos] = Color(buffer[pixel_pos_in_buffer], buffer[pixel_pos_in_buffer + 1], buffer[pixel_pos_in_buffer + 2]);
        }
    }

    stbi_image_free(buffer);
}

std::vector<std::vector<Color>> & Scene::Get_background_pic() {
    return _background_pic;
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

Color Scene::Background_pixel(const Vector3<double> &ray_to_pixel) const {
    size_t bg_x = acos(Vector3<double>(ray_to_pixel.x, 0, ray_to_pixel.z).normalized() * Vector3<double>(0, 0, 1)) / M_PI *
            _background_pic[0].size() / 2 * (ray_to_pixel.x / fabs(ray_to_pixel.x)) +
            _background_pic[0].size() / 2;
    size_t bg_y = (1 - acos(Vector3<double>(0, ray_to_pixel.y, ray_to_pixel.z).normalized() * Vector3<double>(0, 1, 0)) / M_PI) *
            static_cast<double>(_background_pic.size());

    return _background_pic[bg_y][bg_x];
}
