#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <memory>
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
float ambient_coeff = 0.4;

vec3 operator* (const float &a, const vec3 &b) {
    __m128 result;
    result = _mm_mul_ps(b.vector, _mm_set_ps(a, a, a, a));
    return vec3(result[0], result[1], result[2]);
}

void load_scene_file(const char * file_name) {
    return;
}

void print_ppm_image(const char * file_name, int width, int height, COLOR ** image, int max_intensity) {
    std::ofstream ppm_file;
    std::string ppm_string = "";
    float red, green, blue;

    ppm_file.open(file_name);

    ppm_string += "P3\n";
    ppm_string += std::to_string(width) + " " + std::to_string(height) + "\n";
    ppm_string += std::to_string(max_intensity) + "\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            red = image[j][i].red;
            green = image[j][i].green;
            blue = image[j][i].blue;
            ppm_string += std::to_string((int)(std::min(red * max_intensity, (float)max_intensity))) + " ";
            ppm_string += std::to_string((int)(std::min(green * max_intensity, (float)max_intensity))) + " ";
            ppm_string += std::to_string((int)(std::min(blue * max_intensity, (float)max_intensity))) + "\n";
        }
    }

    ppm_file << ppm_string << std::endl;

    ppm_file.close();
    return;
}

bool check_occlusion(const vec3 &origin, const vec3 &direction, const std::vector<std::shared_ptr<GraphicsObj>> &objects) {
    for (int i = 0; i < objects.size(); i++)
        if (objects[i]->rayIntersection(origin, direction))
            return true;
    return false;
}

COLOR trace_ray(const vec3 &origin, const vec3 &direction, const std::vector<std::shared_ptr<GraphicsObj>> &objects, int ray_bounce_count, int prev_id) {
    float distance = 99999999.99f;
    float color = 1.0f;
    float surface_red, surface_green, surface_blue;
    float occlusion = 1.0f;

    COLOR surface_color;
    COLOR reflection_color;

    vec3 ray_intersection = vec3(0.0f, 0.0f, 0.0f);
    vec3 surface_normal = vec3(0.0f, 0.0f, 0.0f);
    vec3 reflected_ray = vec3(0.0f, 0.0f, 0.0f);
    vec3 surface_to_light = vec3(0.0f, 0.0f, 0.0f);

    std::shared_ptr<GraphicsObj> curr_object;
    std::shared_ptr<GraphicsObj> object_in_view;

    if (ray_bounce_count == 5)
        return {0.0, 0.0, 0.0};

    for (int i = 0; i < objects.size(); i++) {
        curr_object = objects[i];
        if (curr_object->rayIntersection(origin, direction) && curr_object->getId() != prev_id) {
            ray_intersection = curr_object->calculateRayIntersection(origin, direction);
            if ((ray_intersection - origin).magnitude() < distance) {
                distance = (ray_intersection - origin).magnitude();
                object_in_view = curr_object;
                surface_normal = object_in_view->calculateSurfaceNormal(ray_intersection);
            }
        }
    }

    if (object_in_view == nullptr)
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
            surface_red *= occlusion * pow(std::max(vec3::dot(surface_normal, surface_to_light), 0.0f), 2.0f);
            surface_green *= occlusion * pow(std::max(vec3::dot(surface_normal, surface_to_light), 0.0f), 2.0f);
            surface_blue *= occlusion * pow(std::max(vec3::dot(surface_normal, surface_to_light), 0.0f), 2.0f);
            return {surface_red + ambient_coeff, surface_green + ambient_coeff, surface_blue + ambient_coeff};
        }
        else if (object_in_view->obj_material == MATERIAL::MIRROR) {
            reflected_ray = direction - 2.0*vec3::dot(direction, surface_normal)*surface_normal;
            reflection_color = trace_ray(vec3::normalize(ray_intersection), vec3::normalize(reflected_ray), objects, ray_bounce_count + 1, object_in_view->getId());
            return reflection_color;
        }
        else if (object_in_view->obj_material == MATERIAL::GLASS) {
            return white;
        }
    }
}

int main() {

    std::cout << "raytracer testing start" << std::endl;

    float screen_offset, fov;
    fov = 100.0f;
    screen_offset = 0.4195f;
    vec3 origin = vec3(0.0f, 8.5f, -4.0f);
    vec3 gaze = vec3::normalize(vec3(0.0f, 0.0f, 1.0f) - origin);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 eye_horiz_axis = vec3::normalize(vec3::cross(gaze, up));
    vec3 eye_vert_axis = vec3::normalize(vec3::cross(eye_horiz_axis, gaze));
    vec3 camera_ray_horiz_inc = eye_horiz_axis/((float)WIDTH);
    vec3 camera_ray_vert_inc = eye_vert_axis/((float)HEIGHT);
    vec3 upper_left_pixel = camera_ray_horiz_inc * ((float) WIDTH/2.0f) + camera_ray_vert_inc * ((float) HEIGHT/2.0) - camera_ray_horiz_inc/2.0f - camera_ray_vert_inc/2.0f;
    vec3 curr_ray_dir = vec3(0.0f, 0.0f, 0.0f);

    
    vec3 sphere1_position = vec3(1.0f, 2.1f, 2.0f);
    std::shared_ptr<Sphere> sphere(new Sphere(sphere1_position, MATERIAL::MIRROR, 1.0f));
    std::cout << "Sphere id: " << sphere->getId() << std::endl;
    
    vec3 sphere2_position = vec3(-2.0f, 3.0f, 3.5f);
    std::shared_ptr<Sphere> sphere2(new Sphere(sphere2_position, MATERIAL::DIELECTRIC, 1.0f));
    std::cout << "Sphere2 id: " << sphere2->getId() << std::endl;

    vec3 plane1_position = vec3(0.0f, -1.5f, 0.0f);
    vec3 plane1_normal = vec3(0.0f, 1.0f, 0.0f);
    std::shared_ptr<Plane> plane(new Plane(plane1_position, MATERIAL::DIELECTRIC, plane1_normal));
    std::cout << "Plane id: " << plane->getId() << std::endl;
    
    std::vector<std::shared_ptr<GraphicsObj>> objects;

    objects.push_back(sphere);
    objects.push_back(sphere2);
    objects.push_back(plane);

    COLOR ** image = new COLOR*[WIDTH];
    for (int i = 0; i < WIDTH; i++)
        image[i] = new COLOR[HEIGHT];

    for (float i = 0; i < (float)HEIGHT; i = i + 1.0f) {
        for (float j = 0; j < (float)WIDTH; j = j + 1.0f) {
            curr_ray_dir = vec3::normalize(gaze*screen_offset + upper_left_pixel - camera_ray_horiz_inc*j - camera_ray_vert_inc*i);
            image[(int)j][(int)i] = trace_ray(origin, curr_ray_dir, objects, 0, -1);
        }
    }

    std::cout << "DONE" << std::endl;

    std::cout << "eye_horiz_axis = ";
    eye_horiz_axis.print();
    std::cout << "eye_vert_axis = ";
    eye_vert_axis.print();
    std::cout << "upper_left_pixel = ";
    upper_left_pixel.print();
    (gaze*screen_offset + upper_left_pixel - camera_ray_horiz_inc*639.0f - camera_ray_vert_inc*479.0f).print();

    print_ppm_image("test.ppm", WIDTH, HEIGHT, image, 255);

    return 0;
}