#include <iostream>
#include <cmath>
#include <algorithm>
#include "vec3.h"
#include "GraphicsObj.h"
#include "Plane.h"

Plane::Plane(vec3 &position, MATERIAL obj_material, vec3 &normal) : GraphicsObj(position, obj_material),
normal(normal) {
}

Plane::~Plane() {
    std::cout << "Destroy Plane" << std::endl;
}

bool Plane::rayIntersection(const vec3 &origin, const vec3 &direction) {
    if (vec3::dot(direction, normal) > 0)
        return false;
    else
        return true;
}

vec3 Plane::calculateSurfaceNormal(const vec3 &surface_pos) {
    return normal;
}

vec3 Plane::calculateRayIntersection(const vec3 &origin, const vec3 &direction) {
    float n_dot_dir;
    float distance;
    n_dot_dir = vec3::dot(normal, direction);
    if (n_dot_dir > 0)
        return vec3(0.0, 0.0, 0.0);
    distance = vec3::dot(normal, (position - origin))/n_dot_dir;
    return (direction * distance) + origin;
}

COLOR Plane::getSurfaceColor(const vec3 &surface_pos) {
    if (std::abs(surface_pos.x) - ((int)std::abs(surface_pos.x)) < 0.5)
        if (std::abs(surface_pos.z) - ((int)std::abs(surface_pos.z)) >= 0.5)
            return {1.0, 1.0, 1.0};
        else
            return {0.0, 0.0, 0.0};
    else
        if (std::abs(surface_pos.z) - ((int)std::abs(surface_pos.z)) < 0.5)
            return {1.0, 1.0, 1.0};
        else
            return {0.0, 0.0, 0.0};
}