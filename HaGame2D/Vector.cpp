#include "stdafx.h"
#include "Vector.h"
#include <math.h>

Vector::Vector()
{
	x = 0; y = 0; z = 0;
}

Vector::Vector(float _x, float _y) {
	x = _x; y = _y; z = 0;
}

Vector::Vector(float _x, float _y, float _z) {
	x = _x; y = _y; z = _z;
}

Vector::~Vector()
{
}

Vector Vector::operator + (const Vector &b) {
	return Vector(x + b.x, y + b.y, z + b.z);
}

Vector Vector::operator * (float scalar) {
	return Vector(x * scalar, y * scalar, z * scalar);
}

Vector Vector::operator - (const Vector &b) {
	return Vector(x - b.x, y - b.y, z - b.z);
}

bool Vector::operator == (const Vector &b) {
	return (x == b.x && y == b.y && z == b.z);
}

bool Vector::operator != (const Vector &b) {
	return (x != b.x || y != b.y || z != b.z);
}

Vector & Vector::operator += (const Vector &b) {
	this->x += b.x; 
	this->y += b.y;
	this->z += b.z;

	return *this;
}

Vector & Vector::operator -= (const Vector &b) {
	this->x -= b.x;
	this->y -= b.y;
	this->z -= b.z;

	return *this;
}

float Vector::magnitude() {
	return sqrt((x * x) + (y * y) + (z * z));
}

void Vector::normalize() {
	float mag = magnitude();
	x = x / mag;
	y = y / mag;
	z = z / mag;
}

Vector Vector::normalized() {
	float mag = magnitude();
	if (mag == 0) {
		return Vector::Zero();
	}
	else {
		return Vector(x / mag, y / mag, z / mag);
	}
}

float Vector::dot(Vector b) {
	return (x * b.x) + (y * b.y) + (z * b.z);
}

Vector Vector::Identity() {
	return Vector(1, 1, 1);
}

Vector Vector::Zero() {
	return Vector(0, 0, 0);
}

void Vector::display() {
	std::cout << "Vector <" << x << ", " << y << ", " << z << ">" << std::endl;
}