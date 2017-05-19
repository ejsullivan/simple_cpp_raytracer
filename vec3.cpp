#include <iostream>
#include <cmath>
#include "vec3.h"

vec3::vec3(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

double vec3::magnitude() {
	return sqrt(x*x + y*y + z*z);
}

double vec3::dot(vec3 a, vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3::cross(vec3 a, vec3 b) {
	double result_x, result_y, result_z;
	result_x = a.y*b.z - a.z*b.y;
	result_y = a.z*b.x - a.x*b.z;
	result_z = a.x*b.y - a.y*b.x;
	return vec3(result_x, result_y, result_z); 
}

vec3 vec3::normalize(vec3 a) {
	double length = a.magnitude();
	return vec3(a.x/length, a.y/length, a.z/length);
}

vec3 vec3::operator+ (const vec3& param) {
	return vec3(x + param.x, y + param.y, z + param.z);
}

/*
int main() {
	vec3 vector = vec3(1.0, 0.0, 0.0);
	std::cout << vector.magnitude() << std::endl;
	vector = vector + vec3(1.0, 1.0, 1.0);
	std::cout << vec3::dot(vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0)) << std::endl;
	std::cout << vec3::cross(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0)).z << std::endl;
	std::cout << "length: " << vec3::normalize(vec3(6.4, 8.6, 1.2)).magnitude() << std::endl;
}*/
