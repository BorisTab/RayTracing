#include <scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

__device__ Light::Light(const Vector3<double> &position, double intensity):
    position(position.x, -position.y, position.z),
    intensity(intensity) {}

void SceneF::Set_canvas(Scene& scene, size_t height, size_t width, const Color &bg_color) {
    CanvasF::Setup(scene._canvas, height, width, bg_color);
}

void SceneF::Set_camera(Scene& scene, const Vector3<double> &camera_pos, double fov) {
    scene._camera_pos = camera_pos;
    scene._fov = fov;
}

void SceneF::Set_lights(Scene& scene, const std::vector<Light> &lights) {
    scene._lights = lights;
}

void SceneF::Set_background_pic(Scene& scene, const char* filepath, int desired_channels) {
    int width = 0;
    int height = 0;
    int channels_in_file = 0;
    unsigned char* buffer = stbi_load(filepath, &width, &height, &channels_in_file, desired_channels);

    printf("channels in envmap file: %d, size: %d x %d\n", channels_in_file, width, height);

    scene._background_pic.resize(height);
    int pixel_pos_in_buffer = 0;
    for (int line_num = 0; line_num < height; ++line_num) {
        scene._background_pic[line_num].resize(width);

        for (int pixel_pos = 0; pixel_pos < width; ++pixel_pos) {
            pixel_pos_in_buffer += 3;
            scene._background_pic[line_num][pixel_pos] = Color(buffer[pixel_pos_in_buffer], buffer[pixel_pos_in_buffer + 1], buffer[pixel_pos_in_buffer + 2]);
        }
    }

    scene._background_pic_height = height;
    scene._background_pic_width = width;

    stbi_image_free(buffer);
}

//std::vector<std::vector<Color>> & Scene::Get_background_pic() {
//    return _background_pic;
//}
//
//const std::vector<Light>& Scene::Get_lights() {
//    return _lights;
//}

__device__ Vector3<double> SceneF::Ray_to_pixel_from_camera(Scene& scene, size_t x, size_t y) {
    double z_pos = scene._camera_pos.z + 1;

    double canvas_height = 2 * z_pos * std::tan(scene._fov / 2.);
    double canvas_width = canvas_height * scene._canvas._width / (scene._canvas._height * 1.);

    double x_pos = ((x + 0.5) / (scene._canvas._width  * 1.) - 0.5) * canvas_width;
    double y_pos = ((y + 0.5) / (scene._canvas._width * 1.) - 0.5) * canvas_height;

    Vector3<double> ray_to_pixel = {x_pos, y_pos, z_pos};

    return ray_to_pixel;
}

//Canvas& Scene::Get_canvas() {
//    return _canvas;
//}
//
//const Vector3<double> & Scene::Get_camera_pos() {
//    return _camera_pos;
//}

int SceneF::Save_canvas_to_png(Scene& scene, const char* filepath) {
    return CanvasF::Save_to_png(scene._canvas, filepath);
}

__device__ SimpleColor SceneF::Background_pixel(const Scene& scene, const Vector3<double> &ray_to_pixel) {
    Vector3<double> z_vector = {0, 0, 1};
    Vector3<double> y_vector = {0, 1, 0};

    Vector3<double> xz_vector = {ray_to_pixel.x, 0, ray_to_pixel.z};
    Vector3<double> yz_vector = {0, ray_to_pixel.y, ray_to_pixel.z};

    size_t bg_x = acos(Vec3::normalize(xz_vector) * z_vector) / M_PI *
            scene._background_pic_width / 2 * (ray_to_pixel.x / fabs(ray_to_pixel.x)) +
            scene._background_pic_width / 2;

    size_t bg_y = (1 - acos(Vec3::normalize(yz_vector) * y_vector) / M_PI) *
            static_cast<double>(scene._background_pic_height);

    return scene._background_pic[std::min(bg_y, scene._background_pic_height - 1)][std::min(bg_x, scene._background_pic_width - 1)];
}
