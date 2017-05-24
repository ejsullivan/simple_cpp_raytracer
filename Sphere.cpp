#include <iostream>
#include <cmath>
#include <algorithm>
#include "vec3.h"
#include "GraphicsObj.h"
#include "Sphere.h"

Sphere::Sphere(vec3 * position, double radius) : GraphicsObj(position) {
    this->radius = radius;
}

bool Sphere::rayIntersection(vec3 origin, vec3 direction) {
    double test_result;
    test_result = pow(vec3::dot(direction, (origin - *position)), 2.0) - pow((origin - *position).magnitude(), 2.0) + pow(radius, 2.0);
    if (test_result < 0)
        return false;
    else
        return true;
}

vec3 Sphere::calculateSurfaceNormal(vec3 surface_pos) {
    return vec3::normalize(surface_pos - *position);
}

vec3 Sphere::calculateRayIntersection(vec3 origin, vec3 direction) {
    double b, root, distance;
    root = pow(vec3::dot(direction, (origin - *position)), 2.0) - pow((origin - *position).magnitude(), 2.0) + pow(radius, 2.0);
    b = -(vec3::dot(direction, (origin - *position)));
    distance = std::min(b + root, b - root);
    return (direction * distance) + origin;
}

COLOR Sphere::getSurfaceColor(vec3 surface_pos) {
    return {1.0, 1.0, 1.0};
}