#pragma once
#include <iostream>

class Vector
{
public:
	float x = NULL;
	float y = NULL;
	float z = NULL;

	Vector();
	Vector(float x, float y);
	Vector(float x, float y, float z);
	~Vector();

	void normalize();
	Vector normalized();

	float magnitude();

	float dot(Vector b);

	Vector operator + (const Vector& b);
	Vector operator - (const Vector& b);
	Vector operator * (float scalar);
	bool operator == (const Vector & b);
	bool operator != (const Vector & b);

	Vector& operator += (const Vector & b);
	Vector& operator -= (const Vector & b);

	bool isContainedBy(float x, float y, float w, float h);

	void display();

	static Vector Identity();
	static Vector Zero();
};

struct Transform {
	Vector position;
	Vector scale;
	float rotation;
};