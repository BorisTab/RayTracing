#ifndef RAYTRACING_CANVAS_H
#define RAYTRACING_CANVAS_H


#include <geometry.h>
#include <vector>

struct Canvas {
//    Canvas() = default;

//    Canvas(size_t height, size_t width, const Color& bg_color = Color(255, 255, 255));

//    void Setup(size_t height, size_t width, const Color& bg_color = Color(255, 255, 255));

//    int Save_to_png(const char* filepath);

//    size_t Height() const;
//
//    size_t Width() const;

    uchar3* pixels;

    size_t _height = 0;
    size_t _width = 0;
    int _components = 3;
};

namespace CanvasF {
    void Setup(Canvas& canvas, size_t height, size_t width, const Color& bg_color = Color(255, 255, 255));

    void Delete_canvas(Canvas& canvas);

    int Save_to_png(Canvas& canvas, const char* filepath);

}

#endif //RAYTRACING_CANVAS_H
