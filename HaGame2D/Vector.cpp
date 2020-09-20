#include "stdafx.h"
#include "Vector.h"
#include <math.h>

Vector::Vector()
{
	x = 0; y = 0; z = 0;
}

Vector::Vector(double _x, double _y) {
	x = _x; y = _y; z = 0;
}

Vector::Vector(double _x, double _y, double _z) {
	x = _x; y = _y; z = _z;
}

Vector::~Vector()
{
}

Vector Vector::operator + (const Vector &b) {
	return Vector(x + b.x, y + b.y, z + b.z);
}

Vector Vector::operator * (double scalar) {
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

Vector Vector::cross(Vector b)
{
	Vector vect;
	vect.x = y * b.z - z * b.y;
	vect.y = z * b.x - x * b.z;
	vect.z = x * b.y - y * b.x;
	return vect;
}

bool Vector::isContainedBy(double _x, double _y, double _w, double _h)
{
	if (x >= _x && x <= _x + _w && y >= _y && y <= _y + _h) {
		return true;
	}
	else {
		return false;
	}
}

double Vector::magnitude() {
	return sqrt((x * x) + (y * y) + (z * z));
}

void Vector::normalize() {
	double mag = magnitude();
	if (mag != 0) {
		x = x / mag;
		y = y / mag;
		z = z / mag;
	}
}

Vector Vector::normalized() {
	double mag = magnitude();
	if (mag == 0) {
		return Vector::Zero();
	}
	else {
		return Vector(x / mag, y / mag, z / mag);
	}
}

double Vector::dot(Vector b) {
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

std::string Vector::toString() {
	return "<" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ">";
}
