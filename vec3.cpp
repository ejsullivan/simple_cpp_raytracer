#include <iostream>
#include <cmath>
#include "vec3.h"

vec3::vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	vector = _mm_set_ps(0.0f, z, y, x);
}

float vec3::magnitude() const {
	__m128 mult_result;
	__m128 result;
	__m128 temp;

	mult_result = _mm_mul_ps(vector, vector);
	temp = _mm_movehdup_ps(mult_result);
	result = _mm_add_ps(mult_result, temp);
	temp = _mm_movehl_ps(temp, result);
	result = _mm_add_ss(result, temp);
	return sqrt(_mm_cvtss_f32(result));
}

void vec3::print() {
	std::cout << "x: " << vector[0] << " y: " << vector[1] << " z: " << vector[2] << std::endl;
}

float vec3::dot(const vec3& a, const vec3& b) {
	__m128 mult_result;
	__m128 result;
	__m128 temp;

	mult_result = _mm_mul_ps(a.vector, b.vector);
	temp = _mm_movehdup_ps(mult_result);
	result = _mm_add_ps(mult_result, temp);
	temp = _mm_movehl_ps(temp, result);
	result = _mm_add_ss(result, temp);
	return _mm_cvtss_f32(result);
}

vec3 vec3::cross(const vec3& a, const vec3& b) {
	__m128 lhs;
	__m128 rhs;
	__m128 result;

	lhs = _mm_mul_ps(
		_mm_shuffle_ps(a.vector, a.vector, _MM_SHUFFLE(3, 0, 2, 1)),
		_mm_shuffle_ps(b.vector, b.vector, _MM_SHUFFLE(3, 1, 0, 2))
	);
	rhs = _mm_mul_ps(
		_mm_shuffle_ps(a.vector, a.vector, _MM_SHUFFLE(3, 1, 0, 2)), 
		_mm_shuffle_ps(b.vector, b.vector, _MM_SHUFFLE(3, 0, 2, 1))
	);
	result = _mm_sub_ps(lhs, rhs);

	//return vec3((float)_mm_extract_ps(result, 3), (float)_mm_extract_ps(result, 2), (float)_mm_extract_ps(result, 1));
	return vec3(result[0], result[1], result[2]); // Should not do this because it violates strong aliasing rules
}

vec3 vec3::normalize(const vec3& a) {
	double length = a.magnitude();
	__m128 denominator = _mm_set_ps(length, length, length, length);
	__m128 result = _mm_div_ps(a.vector, denominator);
	return vec3(result[0], result[1], result[2]); // Should not do this because it violates strong aliasing rules
	//return vec3(a.x/length, a.y/length, a.z/length);
}

vec3 vec3::operator+ (const vec3& param) const {
	__m128 result;
	result = _mm_add_ps(vector, param.vector);
	return vec3(result[0], result[1], result[2]); // Should not do this because it violates strong aliasing rules
	//return vec3(x + param.x, y + param.y, z + param.z);
}

vec3 vec3::operator- (const vec3& param) const {
	__m128 result;
	result = _mm_sub_ps(vector, param.vector);
	return vec3(result[0], result[1], result[2]); // Should not do this because it violates strong aliasing rules
	//return vec3(x - param.x, y - param.y, z - param.z);
}

vec3 vec3::operator* (const double& param) const {
	__m128 result;
	__m128 denominator = _mm_set_ps(param, param, param, param);
	result = _mm_mul_ps(vector, denominator);
	return vec3(result[0], result[1], result[2]); // Should not do this because it violates strong aliasing rules
	//return vec3(this->x*param, this->y*param, this->z*param);
}

vec3 vec3::operator/ (const double& param) const {
	__m128 result;
	__m128 denominator = _mm_set_ps(param, param, param, param);
	result = _mm_div_ps(vector, denominator);
	return vec3(result[0], result[1], result[2]); // Should not do this because it violates strong aliasing rules
	//return vec3(this->x/param, this->y/param, this->z/param);
}