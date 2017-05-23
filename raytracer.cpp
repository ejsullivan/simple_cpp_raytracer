#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "vec3.h"
#include "GraphicsObj.h"
#include "Sphere.h"
#include "Plane.h"

#define WIDTH 640
#define HEIGHT 480

typedef struct color {
    double red;
    double green;
    double blue;
} COLOR;

COLOR white = {1.0, 1.0, 1.0};
COLOR black = {0.0, 0.0, 0.0};

void load_scene_file(char * file_name) {
    return;
}

void print_ppm_image(char * file_name, int width, int height, COLOR ** image, int max_intensity) {
    std::ofstream ppm_file;
    ppm_file.open(file_name);

    ppm_file << "P3" << std::endl;
    ppm_file << width << " " << height << std::endl;
    ppm_file << max_intensity << std::endl;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            ppm_file << image[j][i].red * max_intensity << " " << image[j][i].green * max_intensity << " " << image[j][i].blue * max_intensity << std::endl;

    ppm_file.close();
    return;
}

vec3 operator* (const double& a, const vec3& b) {
    return vec3(a*b.x, a*b.y, a*b.z);
}

COLOR * trace_ray(vec3 origin, vec3 direction) {
    return NULL;
}

int main() {

    std::cout << "raytracer testing start" << std::endl;

    double screen_offset, fov;
    fov = 100.0;
    screen_offset = 0.4195;
    vec3 origin = vec3(0.0, 0.0,-1.0);
    vec3 gaze = vec3::normalize(vec3(0.0, 0.0, 1.0));
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 eye_horiz_axis = vec3::normalize(vec3::cross(gaze, up));
    vec3 eye_vert_axis = vec3::normalize(vec3::cross(eye_horiz_axis, gaze));
    vec3 camera_ray_horiz_inc = eye_horiz_axis/((double)WIDTH);
    vec3 camera_ray_vert_inc = eye_vert_axis/((double)HEIGHT);
    vec3 upper_left_pixel = camera_ray_horiz_inc * ((double) WIDTH/2.0) + camera_ray_vert_inc * ((double) HEIGHT/2.0) - camera_ray_horiz_inc/2.0 - camera_ray_vert_inc/2.0;
    vec3 curr_ray_dir = vec3(0.0, 0.0, 0.0);

    Sphere sphere = Sphere(new vec3(0.0, 0.0, 3.0), 1.0);

    COLOR ** image = new COLOR*[WIDTH];
    for (int i = 0; i < WIDTH; i++)
        image[i] = new COLOR[HEIGHT];

    for (double i = 0; i < (double)HEIGHT; i = i + 1.0) {
        for (double j = 0; j < (double)WIDTH; j = j + 1.0) {
            curr_ray_dir = vec3::normalize(gaze*screen_offset + upper_left_pixel - camera_ray_horiz_inc*j - camera_ray_vert_inc*i);
            if (sphere.rayIntersection(origin, curr_ray_dir))
                image[(int)j][(int)i] = {1.0, 1.0, 1.0};
            else
                image[(int)j][(int)i] = {0.0, 0.0, 0.0};
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