#include <iostream>
#include <cmath>
#include "raytracer.h"
#include "vec3.h"
#include "GraphicsObj.h"

int GraphicsObj::curr_id = 0;

GraphicsObj::GraphicsObj(vec3 &position, MATERIAL obj_material) : position(position){
    this->obj_material = obj_material;
    this->id = curr_id;
    curr_id++;
}

int GraphicsObj::getId() {
    return this->id;
}

float GraphicsObj::calculateDistanceFrom(const vec3 &point) {
    return (point - position).magnitude();
    //return sqrt(pow(point->x - position->x, 2.0) + pow(point->y - position->y, 2.0) + pow(point->z - position->z, 2.0));
}