#include <geometry.h>

__device__ SimpleColor operator*(SimpleColor& color, const double scalar) {
    color.r = (double)color.r * scalar;
    color.g = (double)color.r * scalar;
    color.b = (double)color.r * scalar;

    return color;
}

__device__ SimpleColor operator*(SimpleColor&& color, const double scalar) {
    color.r = (double)color.r * scalar;
    color.g = (double)color.r * scalar;
    color.b = (double)color.r * scalar;

    return color;
}

__device__ SimpleColor operator+(SimpleColor& color1, const SimpleColor& color2) {
    color1.r += color2.r;
    color1.g += color2.g;
    color1.b += color2.b;

    return color1;
}

__device__ SimpleColor operator+(SimpleColor&& color1, const SimpleColor& color2) {
    color1.r += color2.r;
    color1.g += color2.g;
    color1.b += color2.b;

    return color1;
}

__device__ void copy_simple_color(const SimpleColor& from, SimpleColor& to) {
    to.r = from.r;
    to.g = from.g;
    to.b = from.b;
}
