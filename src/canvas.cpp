#include <canvas.h>
#include <memory>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

Canvas::Canvas(size_t height, size_t width, const Color &bg_color):
    pixels(height * width, bg_color),
    _height(height),
    _width(width) {}

void Canvas::Setup(size_t height, size_t width, const Color &bg_color) {
    _height = height;
    _width = width;

    pixels = std::vector(width * height, bg_color);
}

size_t Canvas::Height() const {
    return _height;
}

size_t Canvas::Width() const {
    return _width;
}



int Canvas::Save_to_png(const char *filepath) {
    auto row_data = std::make_unique<unsigned char[]>(_height * _width * _components);
    size_t row_position = 0;

    for (size_t line = 0; line < _height; ++line) {
        for (size_t pixel_pos = 0; pixel_pos < _width; ++pixel_pos) {
            auto& pixel = pixels[line * _width + pixel_pos];

            row_data[row_position++] = pixel.x;
            row_data[row_position++] = pixel.y;
            row_data[row_position++] = pixel.z;
        }
    }

    return stbi_write_png(filepath,
                          _width,
                          _height,
                          _components,
                          row_data.get(),
                          _width * _components);
}
