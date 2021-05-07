#include <iostream>

#include <scene.h>
#include <sphere.h>

int main() {
    std::vector<Light> lights = {Light({-1000, 1000, 500}, 1)};

    Scene scene;
    scene.Set_canvas(1080, 1920, {127, 199, 255});
    scene.Set_camera({0., 0., 0.}, 70/180. * M_PI);
    scene.Set_lights(lights);

    Sphere sphere1({800, 200, 2100},400, {Color(102, 0, 204), {0.3, 1, 0}, 10});
    Sphere sphere2({300, -50, 2000},400, {Color(255, 165, 0), {0.3, 1, 0.3}, 30});
    Sphere sphere3({-30, -100,500}, 100, {Color(102, 0, 204), {0.3, 1, 1}, 30});
    Sphere sphere4({-200,0,   1000},200, {Color(255, 165, 0), {0.4, 1, 1}, 50});

    std::vector<Sphere> spheres;
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);

    Sphere::Set_spheres_on_scene(scene, spheres);

    scene.Save_canvas_to_png("../pictures/pic1.png");

    return 0;
}
