#include <iostream>
#include <cmath>
#include "vec3.h"
#include "GraphicsObj.h"
#include "Sphere.h"

Sphere::Sphere(vec3 * position, double radius) : GraphicsObj(position) {
    this->radius = radius;
}

bool Sphere::rayIntersection(vec3 origin, vec3 direction) {
    return false;
}

vec3 Sphere::calculateSurfaceNormal(vec3 surface_pos) {
    return vec3(0.0, 0.0, 0.0);
}

vec3 Sphere::calculateRayIntersection(vec3 origin, vec3 direction) {
    return vec3(0.0, 0.0, 0.0);
}