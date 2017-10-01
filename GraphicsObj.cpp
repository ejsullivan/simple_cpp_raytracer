#include <iostream>
#include <cmath>
#include "raytracer.h"
#include "vec3.h"
#include "GraphicsObj.h"

GraphicsObj::GraphicsObj(vec3 * position, MATERIAL obj_material) {
    this->position = position;
    this->obj_material = obj_material;
}

float GraphicsObj::calculateDistanceFrom(vec3 * point) {
    return (*point - *position).magnitude();
    //return sqrt(pow(point->x - position->x, 2.0) + pow(point->y - position->y, 2.0) + pow(point->z - position->z, 2.0));
}