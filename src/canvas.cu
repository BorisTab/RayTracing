#include <canvas.h>
#include <memory>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

//Canvas::Canvas(size_t height, size_t width, const Color &bg_color):
//    pixels(height * width, bg_color),
//    _height(height),
//    _width(width) {}

void CanvasF::Setup(Canvas& canvas, size_t height, size_t width, const Color &bg_color) {
    canvas._height = height;
    canvas._width = width;

    canvas.pixels = std::vector<Color>(width * height, bg_color);
}

//size_t Canvas::Height() const {
//    return _height;
//}
//
//size_t Canvas::Width() const {
//    return _width;
//}



int CanvasF::Save_to_png(Canvas& canvas, const char *filepath) {
    auto row_data = std::make_unique<unsigned char[]>(canvas._height * canvas._width * canvas._components);
    size_t row_position = 0;

    for (size_t line = 0; line < canvas._height; ++line) {
        for (size_t pixel_pos = 0; pixel_pos < canvas._width; ++pixel_pos) {
            auto& pixel = canvas.pixels[line * canvas._width + pixel_pos];

            row_data[row_position++] = pixel.x;
            row_data[row_position++] = pixel.y;
            row_data[row_position++] = pixel.z;
        }
    }

    return stbi_write_png(filepath,
                          canvas._width,
                          canvas._height,
                          canvas._components,
                          row_data.get(),
                          canvas._width * canvas._components);
}
