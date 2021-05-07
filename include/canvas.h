#ifndef RAYTRACING_CANVAS_H
#define RAYTRACING_CANVAS_H


#include <geometry.h>
#include <vector>

class Canvas {
public:
    Canvas() = default;

    Canvas(size_t height, size_t width, const Color& bg_color = Color(255, 255, 255));

    void Setup(size_t height, size_t width, const Color& bg_color = Color(255, 255, 255));

    int Save_to_png(const char* filepath);

    size_t Height() const;

    size_t Width() const;

public:
    std::vector<std::vector<Color>> pixels;

private:
    size_t _height = 0;
    size_t _width = 0;
    int _components = 3;
};

#endif //RAYTRACING_CANVAS_H
