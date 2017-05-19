#include <iostream>
#include <cmath>
#include "vec3.h"
#include "GraphicsObj.h"

GraphicsObj::GraphicsObj(vec3 * position) {
    this->position = position;
}

double GraphicsObj::calculateDistanceFrom(vec3 * point) {
    return sqrt(pow(point->x - position->x, 2.0) + pow(point->y - position->y, 2.0) + pow(point->z - position->z, 2.0));
}