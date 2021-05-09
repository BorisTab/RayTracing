#include <iostream>

#include <scene.h>
#include <sphere.h>

int main() {
    std::vector<Light> lights = {Light({-20, 20, 20}, 0.5),
                                 Light({30, 50, -25}, 0.5),
//                                 Light({-10, 30, 0}, 0.5)
    };

    Scene scene;
    scene.Set_canvas(1080, 1920, {127, 199, 255});
    scene.Set_camera({0., 0., 0.}, 70/180. * M_PI);
    scene.Set_lights(lights);
    scene.Set_background_pic("../pictures/envmap.jpg");

    Material mirror(Color(255, 255, 255), {0, 0, 10},1000, 0.8, 1, 0);
    Material glass(Color(153, 178, 204), {0, 0, 0.6},100, 0.1, 0.5, 0.8);
    Material purple_matt(Color(102, 0, 204), {0.5, 1, 0}, 10, 0, 1, 0);
    Material orange_semi_matt(Color(255, 165, 0), {0.5, 1, 1}, 30, 0, 1, 0);

    Sphere sphere1({-3,    0,   16},2, purple_matt);
    Sphere sphere2({-1.5, -1.5, 12},2, glass);
    Sphere sphere3({ 1.5, -0.5, 18}, 3, orange_semi_matt);
    Sphere sphere4({7,    5,   18}, 4,  mirror);
//
//        Sphere sphere1({0, 0, 10},5, {Color(102, 0, 204), {0.3, 1, 0}, 10});

    std::vector<Sphere> spheres;
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);

    Sphere::Set_spheres_on_scene(scene, spheres);

    scene.Save_canvas_to_png("../pictures/pic1.png");

//    Vector3<double> light(0.416025, 0.877058, 0);
//    Vector3<double> normal(0, 1, 0);
//
//    auto refracted = Vector3<double>::refract(light.normalized(), normal, 0.75);
//    printf("%g, %g, %g\n", refracted.x, refracted.y, refracted.z);
//    printf("%g\n", -refracted.cos(normal));
//
////    -0.416025, -0.877058, 0
//
//    Vector3 vec = Vector3<double>(2, 3, 0).normalized();
//    printf("%g, %g, %g\n", vec.x, vec.y, vec.z);

    return 0;
}

//int main() {
//    std::vector<Light> lights = {Light({-1000, 1000, 0}, 0.5),
//                                 Light({0, 1000, 0}, 0.5)};
//
//    Scene scene;
//    scene.Set_canvas(1080, 1920, {127, 199, 255});
//    scene.Set_camera({0., 0., 0.}, 70/180. * M_PI);
//    scene.Set_lights(lights);
//
//    Sphere sphere1({800, 200, 2100},400, {Color(102, 0, 204), {0.3, 1, 0}, 10, 0});
//    Sphere sphere2({300, -50, 2000},400, {Color(255, 165, 0), {0.3, 1, 0.3}, 30, 0});
//    Sphere sphere3({-30, -100,500}, 100, {Color(102, 0, 204), {0.3, 1, 1}, 30, 0});
//    Sphere sphere4({-224,120, 400},50, {Color(255, 165, 0), {0.4, 1, 0.8}, 30, 0});
////
////        Sphere sphere1({0, 0, 10},5, {Color(102, 0, 204), {0.3, 1, 0}, 10});
//
//    std::vector<Sphere> spheres;
//    spheres.push_back(sphere1);
//    spheres.push_back(sphere2);
//    spheres.push_back(sphere3);
//    spheres.push_back(sphere4);
//
//    Sphere::Set_spheres_on_scene(scene, spheres);
//
//    scene.Save_canvas_to_png("../pictures/pic1.png");
//
//    return 0;
//}
