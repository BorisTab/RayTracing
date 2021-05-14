#include <iostream>

#include <scene.h>
#include <sphere.h>

int main(int argc, const char* argv[]) {
    std::vector<Light> lights = {Light({-20, 20, 20}, 0.5),
                                 Light({30, 50, -25}, 0.5),
//                                 Light({-10, 30, 0}, 0.5)
    };

    Scene scene;
    SceneF::Set_canvas(scene, 9, 16, {127, 199, 255});
    SceneF::Set_camera(scene, {0., 0., 0.}, 70/180. * M_PI);
    SceneF::Set_lights(scene, lights);
    SceneF::Set_background_pic(scene, "../pictures/envmap.jpg");

    Material mirror;
    double mirror_albedo[3] = {0., 0., 10.};
    MaterialF::MaterialConstruct(mirror, {255, 255, 255}, mirror_albedo, 1000, 0.8, 1, 0);

    Material glass;
    double glass_albedo[3] = {0, 0, 0.6};
    MaterialF::MaterialConstruct(glass, {153, 178, 204}, glass_albedo, 100, 0.1, 0.5, 0.8);

    Material purple_matt;
    double purple_matt_albedo[3] = {0.5, 1, 0.1};
    MaterialF::MaterialConstruct(purple_matt, {102, 0, 204}, purple_matt_albedo, 10, 0, 1, 0);


    Material orange_semi_matt;
    double orange_semi_matt_albedo[3] = {0.5, 1, 1};
    MaterialF::MaterialConstruct(orange_semi_matt, {255, 165, 0}, orange_semi_matt_albedo, 30, 0.05, 1, 0);

    Sphere sphere1({-3,    0,   16},2, purple_matt);
    Sphere sphere2({-1.5, -1.5, 12},2, glass);
    Sphere sphere3({ 1.5, -0.5, 18}, 3, orange_semi_matt);
    Sphere sphere4({ 7,    5,   18}, 4,  mirror);

    std::vector<Sphere> spheres;
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);

    SphereF::Set_spheres_on_scene(scene, spheres);

    SceneF::Save_canvas_to_png(scene, "../pictures/pic1.png");
    SceneF::Delete_scene(scene);

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
//    scene.Set_background_pic("../pictures/envmap.jpg");
//
//    Material mirror(Color(255, 255, 255), {0, 0, 10},1000, 0.8, 1, 0);
//    Material glass(Color(153, 178, 204), {0, 0, 0.6},100, 0.1, 0.5, 0.8);
//    Material purple_matt(Color(102, 0, 204), {0.5, 1, 0}, 10, 0, 1, 0);
//    Material orange_semi_matt(Color(255, 165, 0), {0.5, 1, 1}, 30, 0, 1, 0);
//
//    Sphere sphere1({800, 200, 2100},400, purple_matt);
//    Sphere sphere2({300, -50, 2000},400, glass);
//    Sphere sphere3({-30, -100,500}, 100, orange_semi_matt);
//    Sphere sphere4({-224,120, 400},50, mirror);
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
