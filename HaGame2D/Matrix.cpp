#include "stdafx.h"
#include "Matrix.h"


Matrix::Matrix()
{
	matrix[0] = 0; matrix[1] = 0; matrix[2] = 0; matrix[3] = 0;
}

Matrix::Matrix(float _matrix[4]) {
	for (int i = 0; i < 4; i++) {
		matrix[i] = _matrix[i];
	}
}

Matrix::Matrix(Vector vec1, Vector vec2) {
	matrix[0] = vec1.x;
	matrix[1] = vec1.y;
	matrix[2] = vec2.x;
	matrix[3] = vec2.y;
}

void Matrix::set(int index, float value) {
	if (validIndex(index)) {
		matrix[index] = value;
	}
}

/*
[ 0, 2 ] index =  ((1 - col) * row) + ((row + col + 1) * col)
[ 1, 3 ] weird little index statement but basically just an if else statement in math.
*/
void Matrix::set(int row, int col, float value) {
	if (validVectorIndex(row) && validVectorIndex(col)) {
		matrix[getIndex(row, col)] = value;
	}
}

float Matrix::get(int index) {
	if (validIndex(index)) {
		return matrix[index];
	}
}

float Matrix::get(int row, int col) {
	if (validVectorIndex(row) && validVectorIndex(col)) {
		return matrix[getIndex(row, col)];
	}
}

Matrix Matrix::operator+(const Matrix & other) {
	Matrix result;
	for (int i = 0; i < 4; i++) {
		result.matrix[i] = matrix[i] + other.matrix[i];
	}
	return result;
}

Matrix & Matrix::operator += (const Matrix & other) {
	for (int i = 0; i < 4; i++) {
		matrix[i] += other.matrix[i];
	}
	return *this;
}

Matrix Matrix::operator-(const Matrix & other) {
	Matrix result;
	for (int i = 0; i < 4; i++) {
		result.matrix[i] = matrix[i] - other.matrix[i];
	}
	return result;
}

Matrix & Matrix::operator -= (const Matrix & other) {
	for (int i = 0; i < 4; i++) {
		matrix[i] -= other.matrix[i];
	}
	return *this;
}

Vector Matrix::lMultiply(Vector rowVector) {
	Vector res;
	res.x = matrix[0] * rowVector.x + matrix[1] * rowVector.y;
	res.y = matrix[2] * rowVector.x + matrix[3] * rowVector.y;
	return res;
}

Vector Matrix::rMultiply(Vector colVector) {
	Vector res;
	res.x = matrix[0] * colVector.x + matrix[2] * colVector.y;
	res.y = matrix[1] * colVector.x + matrix[3] * colVector.y;
	return res;
}

bool Matrix::operator == (const Matrix & other) {
	bool equals = true;
	for (int i = 0; i < 4; i++) {
		equals = matrix[i] == other.matrix[i];
	}
	return equals;
}

Matrix Matrix::operator * (const Matrix & other) {
	Matrix res;
	float *A = matrix;
	const float *B = other.matrix;
	res.matrix[0] = A[0] * B[0] + A[2] * B[1];
	res.matrix[1] = A[1] * B[0] + A[3] * B[1];
	res.matrix[2] = A[0] * B[2] + A[2] * B[3];
	res.matrix[3] = A[1] * B[2] + A[3] * B[3];
	return res;
}

Matrix Matrix::operator *(float scalar) {
	Matrix res;
	for (int i = 0; i < 4; i++) {
		res.matrix[i] = scalar * matrix[i];
	}
	return res;
}

float Matrix::determinant() {
	return matrix[0] * matrix[3] - matrix[1] * matrix[2];
}

Matrix::~Matrix()
{
}
