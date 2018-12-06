#pragma once
#include "Vector.h"
#include <string>
/*
A quick and dirty matrix class specifically for 2D.
[ a, x ] represented by [a, b, x, y]
[ b, y ]
*/

class Matrix
{
private:
	float matrix[4];
	bool validIndex(int index) {
		return index >= 0 && index < 4;
	}

	bool validVectorIndex(int index) {
		return index >= 0 && index < 2;
	}

	int getIndex(int row, int col) {
		return ((1 - col) * row) + ((row + col + 1) * col);
	}

public:
	Matrix();
	Matrix(float _matrix[4]);

	/*
		Example:
		Matrix([0, 1], [2, 3]) => [ 0, 1, 2, 3] => [ 0 2 ]
												   [ 1 3 ]
	*/
	Matrix(Vector vec1, Vector vec2);

	static Matrix Identity() {
		return Matrix(Vector(1, 0), Vector(0, 1));
	}

	static Matrix UniformMatrix(float value) {
		Matrix box;
		for (int i = 0; i < 4; i++) {
			box.matrix[i] = value;
		}
		return box;
	}

	static Matrix translationMatrix(Matrix box) {
		box.matrix[2] = 0; box.matrix[3] = 0;
		return box;
	}

	static Matrix translationMatrix(Vector vec) {
		Matrix box;
		box.matrix[0] = vec.x;
		box.matrix[1] = vec.y;
		return box;
	}

	static Matrix translationMatrix(float value) {
		Matrix box;
		box.matrix[0] = value;
		box.matrix[1] = value;
		return box;
	}

	static Matrix sizeMatrix(Matrix box) {
		box.matrix[0] = 0; box.matrix[1] = 0;
		return box;
	}

	static Matrix sizeMatrix(Vector vec) {
		Matrix box;
		box.matrix[2] = vec.x;
		box.matrix[3] = vec.y;
		return box;
	}

	static Matrix sizeMatrix(float value) {
		Matrix box;
		box.matrix[2] = value;
		box.matrix[3] = value;
		return box;
	}

	static Matrix scaleMatrix(float scale) {
		float m[] = { scale, 0, 0, scale };
		return Matrix(m);
	}

	static Matrix scaleMatrix(float scaleX, float scaleY) {
		float m[] = { scaleX, 0, 0, scaleY };
		return Matrix(m);
	}

	void set(int index, float value);
	void set(int row, int col, float value);

	float get(int index);
	float get(int row, int col);

	Matrix operator + (const Matrix &other);
	Matrix & operator += (const Matrix & other);

	Matrix operator - (const Matrix & other);
	Matrix & operator -= (const Matrix & other);

	Matrix operator *(const Matrix & other);
	Matrix operator * (float scalar);

	bool operator == (const Matrix & other);

	Vector lMultiply(Vector rowVector);
	Vector rMultiply(Vector colVector);

	float determinant();

	std::string toString() {
		std::string res = "\n[ " + std::to_string(matrix[0]) + " " + std::to_string(matrix[2]) + " ]\n" + "[ " + std::to_string(matrix[1]) + " " + std::to_string(matrix[3]) + " ]\n";
		return res;
	}

	~Matrix();
};

