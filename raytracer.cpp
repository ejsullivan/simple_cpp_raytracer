#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "vec3.h"
#include "GraphicsObj.h"
#include "Sphere.h"
#include "Plane.h"

#define WIDTH 2000
#define HEIGHT 2000

COLOR white = {1.0, 1.0, 1.0};
COLOR black = {0.0, 0.0, 0.0};
COLOR background = {0.1, 0.1, 0.1};
vec3 light = vec3(1.0, 5.0, 2.0);
double ambient_coeff = 0.4;

vec3 operator* (const double& a, const vec3& b) {
    return vec3(a*b.x, a*b.y, a*b.z);
}

void load_scene_file(char * file_name) {
    return;
}

void print_ppm_image(char * file_name, int width, int height, COLOR ** image, int max_intensity) {
    std::ofstream ppm_file;
    std::string ppm_string = "";
    double red, green, blue;

    ppm_file.open(file_name);

    ppm_string += "P3\n";
    ppm_string += std::to_string(width) + " " + std::to_string(height) + "\n";
    ppm_string += std::to_string(max_intensity) + "\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            red = image[j][i].red;
            green = image[j][i].green;
            blue = image[j][i].blue;
            ppm_string += std::to_string((int)(std::min(red * max_intensity, (double)max_intensity))) + " ";
            ppm_string += std::to_string((int)(std::min(green * max_intensity, (double)max_intensity))) + " ";
            ppm_string += std::to_string((int)(std::min(blue * max_intensity, (double)max_intensity))) + "\n";
        }
    }

    ppm_file << ppm_string << std::endl;

    ppm_file.close();
    return;
}

bool check_occlusion(vec3 origin, vec3 direction, std::vector<GraphicsObj *> &objects) {
    for (int i = 0; i < objects.size(); i++)
        if (objects[i]->rayIntersection(origin, direction))
            return true;
    return false;
}

COLOR trace_ray(vec3 origin, vec3 direction, std::vector<GraphicsObj *> &objects, int ray_bounce_count, GraphicsObj * prev_object) {
    double distance = 99999999.99;
    double color = 1.0;
    double surface_red, surface_green, surface_blue;
    double occlusion = 1.0;

    COLOR surface_color;
    COLOR reflection_color;

    vec3 ray_intersection = vec3(0.0, 0.0, 0.0);
    vec3 surface_normal = vec3(0.0, 0.0, 0.0);
    vec3 reflected_ray = vec3(0.0, 0.0, 0.0);
    vec3 surface_to_light = vec3(0.0, 0.0, 0.0);

    GraphicsObj * curr_object = NULL;
    GraphicsObj * object_in_view = NULL;

    if (ray_bounce_count == 5)
        return {0.0, 0.0, 0.0};
    
    for (int i = 0; i < objects.size(); i++) {
        curr_object = objects[i];
        if (curr_object->rayIntersection(origin, direction) && curr_object != prev_object) {
            ray_intersection = curr_object->calculateRayIntersection(origin, direction);
            if ((ray_intersection - origin).magnitude() < distance) {
                distance = (ray_intersection - origin).magnitude();
                object_in_view = curr_object;
                surface_normal = object_in_view->calculateSurfaceNormal(ray_intersection);
            }
        }
    }

    if (object_in_view == NULL)
        return background;
    else {
        surface_color = object_in_view->getSurfaceColor(ray_intersection);
        surface_red = surface_color.red;
        surface_green = surface_color.green;
        surface_blue = surface_color.blue;

        surface_normal = object_in_view->calculateSurfaceNormal(object_in_view->calculateRayIntersection(origin, direction));
        surface_to_light = vec3::normalize(light - object_in_view->calculateRayIntersection(origin, direction));

        if (object_in_view->obj_material == MATERIAL::DIELECTRIC) {
            if (check_occlusion(ray_intersection, vec3::normalize(light - ray_intersection), objects)) {
                occlusion = 0.2;
            }
            surface_red *= occlusion * pow(std::max(vec3::dot(surface_normal, surface_to_light), 0.0), 2.0);
            surface_green *= occlusion * pow(std::max(vec3::dot(surface_normal, surface_to_light), 0.0), 2.0);
            surface_blue *= occlusion * pow(std::max(vec3::dot(surface_normal, surface_to_light), 0.0), 2.0);
            return {surface_red + ambient_coeff, surface_green + ambient_coeff, surface_blue + ambient_coeff};
        }
        else if (object_in_view->obj_material == MATERIAL::MIRROR) {
            reflected_ray = direction - 2.0*vec3::dot(direction, surface_normal)*surface_normal;
            reflection_color = trace_ray(vec3::normalize(ray_intersection), vec3::normalize(reflected_ray), objects, ray_bounce_count + 1, object_in_view);
            return reflection_color;
        }
        else if (object_in_view->obj_material == MATERIAL::GLASS) {
            return white;
        }
    }
}

int main() {

    std::cout << "raytracer testing start" << std::endl;

    double screen_offset, fov;
    fov = 100.0;
    screen_offset = 0.4195;
    vec3 origin = vec3(0.0, 2.5, -1.0);
    vec3 gaze = vec3::normalize(vec3(0.0, 0.0, 1.0));
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 eye_horiz_axis = vec3::normalize(vec3::cross(gaze, up));
    vec3 eye_vert_axis = vec3::normalize(vec3::cross(eye_horiz_axis, gaze));
    vec3 camera_ray_horiz_inc = eye_horiz_axis/((double)WIDTH);
    vec3 camera_ray_vert_inc = eye_vert_axis/((double)HEIGHT);
    vec3 upper_left_pixel = camera_ray_horiz_inc * ((double) WIDTH/2.0) + camera_ray_vert_inc * ((double) HEIGHT/2.0) - camera_ray_horiz_inc/2.0 - camera_ray_vert_inc/2.0;
    vec3 curr_ray_dir = vec3(0.0, 0.0, 0.0);

    Sphere sphere = Sphere(new vec3(1.0, 2.1, 2.0), MATERIAL::MIRROR, 1.0);
    Sphere sphere2 = Sphere(new vec3(-2.0, 3.0, 3.5), MATERIAL::DIELECTRIC, 1.0);
    Plane plane = Plane(new vec3(0.0, -1.5, 0.0), MATERIAL::DIELECTRIC, new vec3(0.0, 1.0, 0.0));
    std::vector<GraphicsObj *> objects;

    objects.push_back(&sphere);
    objects.push_back(&sphere2);
    objects.push_back(&plane);

    COLOR ** image = new COLOR*[WIDTH];
    for (int i = 0; i < WIDTH; i++)
        image[i] = new COLOR[HEIGHT];

    for (double i = 0; i < (double)HEIGHT; i = i + 1.0) {
        for (double j = 0; j < (double)WIDTH; j = j + 1.0) {
            curr_ray_dir = vec3::normalize(gaze*screen_offset + upper_left_pixel - camera_ray_horiz_inc*j - camera_ray_vert_inc*i);
            image[(int)j][(int)i] = trace_ray(origin, curr_ray_dir, objects, 0, NULL);
        }
    }

    std::cout << "DONE" << std::endl;

    std::cout << "eye_horiz_axis = ";
    eye_horiz_axis.print();
    std::cout << "eye_vert_axis = ";
    eye_vert_axis.print();
    std::cout << "upper_left_pixel = ";
    upper_left_pixel.print();
    (gaze*screen_offset + upper_left_pixel - camera_ray_horiz_inc*639.0 - camera_ray_vert_inc*479.0).print();

    print_ppm_image("test.ppm", WIDTH, HEIGHT, image, 255);

    return 0;
}