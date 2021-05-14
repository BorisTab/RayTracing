#ifndef CUDA_RAY_TRACING_H
#define CUDA_RAY_TRACING_H

#include <geometry.h>
#include <scene.h>
#include <sphere.h>

#include <limits>

namespace Cuda {
    __global__ void Cuda_canvas_intersect(uchar3* d_canvas, Scene& scene, Sphere* spheres, size_t spheres_size, double double_max, Light* lights, size_t lights_size);
}


#endif // CUDA_RAY_TRACING_H