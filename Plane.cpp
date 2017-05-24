#include <iostream>
#include <cmath>
#include <algorithm>
#include "vec3.h"
#include "GraphicsObj.h"
#include "Plane.h"

Plane::Plane(vec3 * position, vec3 * normal) : GraphicsObj(position) {
    this->normal = normal;
}

bool Plane::rayIntersection(vec3 origin, vec3 direction) {
    if (vec3::dot(direction, *normal) > 0)
        return false;
    else
        return true;
}

vec3 Plane::calculateSurfaceNormal(vec3 surface_pos) {
    return *normal;
}

vec3 Plane::calculateRayIntersection(vec3 origin, vec3 direction) {
    double n_dot_dir, distance;
    n_dot_dir = vec3::dot(*normal, direction);
    if (n_dot_dir > 0)
        return vec3(0.0, 0.0, 0.0);
    distance = vec3::dot(*normal, (*position - origin))/n_dot_dir;
    return (direction * distance) + origin;
}