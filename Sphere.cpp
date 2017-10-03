#include <iostream>
#include <cmath>
#include <algorithm>
#include "vec3.h"
#include "GraphicsObj.h"
#include "Sphere.h"

Sphere::Sphere(vec3 &position, MATERIAL obj_material, float radius) : GraphicsObj(position, obj_material) {
    this->radius = radius;
}

Sphere::~Sphere() {
    std::cout << "Destroy Sphere" << std::endl;
}

bool Sphere::rayIntersection(const vec3 &origin, const vec3 &direction) {
    float test_result;
    test_result = pow(vec3::dot(direction, (origin - position)), 2.0) - pow((origin - position).magnitude(), 2.0) + pow(radius, 2.0);
    if (test_result < 0)
        return false;
    else
        return true;
}

vec3 Sphere::calculateSurfaceNormal(const vec3 &surface_pos) {
    return vec3::normalize(surface_pos - position);
}

vec3 Sphere::calculateRayIntersection(const vec3 &origin, const vec3 &direction) {
    float b, root, distance;
    root = pow(vec3::dot(direction, (origin - position)), 2.0) - pow((origin - position).magnitude(), 2.0) + pow(radius, 2.0);
    b = -(vec3::dot(direction, (origin - position)));
    distance = std::min(b + root, b - root);
    return (direction * distance) + origin;
}

COLOR Sphere::getSurfaceColor(const vec3 &surface_pos) {
    return {1.0, 1.0, 1.0};
}