#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "vec3.h"
#include "GraphicsObj.h"
#include "Sphere.h"

#define WIDTH 640
#define HEIGHT 480

typedef struct color {
    double red;
    double green;
    double blue;
} COLOR;

void load_scene_file(char * file_name) {
    return;
}

void print_ppm_image(char * file_name, int width, int height, COLOR ** image, int max_intensity) {
    std::ofstream ppm_file;
    ppm_file.open(file_name);
    ppm_file << "P3";
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

    double screen_offset;
    vec3 origin = vec3(0.0, 0.0, 3.0);
    vec3 gaze = vec3::normalize(vec3(1.0, 1.0, -0.5));

    COLOR ** image = new COLOR*[WIDTH];
    for (int i = 0; i < WIDTH; i++)
        image[i] = new COLOR[HEIGHT];

    print_ppm_image("test.txt", WIDTH, HEIGHT, image, 255);

    return 0;
}