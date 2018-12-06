#pragma once
#include "Vector.h"
#include <math.h>
#include <vector>

const int MAX_ROW_COUNT = 100;
const int MAX_COL_COUNT = 100;

struct GridIndex {
	int x;
	int y;

	bool operator == (const GridIndex &index) {
		return x == index.x && y == index.y;
	}

	bool operator != (const GridIndex &index) {
		return !(*this == index);
	}
};

template <class T>
class Grid
{
private:

	std::vector<std::vector<T>> grid;

public:

	float width;
	float height;

	int rows;
	int cols;

	float rowSize;
	float colSize;

	Grid() {};
	Grid(float _width, float _height, int _rows, int _cols);
	~Grid();

	void setPartitionCount(int _rows, int _cols);

	void initialize(T value);

	void setAll(T value);
	void set(int row, int col, T value);
	void set(GridIndex index, T value);
	void clear(GridIndex index);

	T get(int row, int col);
	T get(GridIndex index);

	/*
		To be used by from within a game object to translate screen positions to positions on the grid.
		This is a useful addition to an otherwise static data structure.
	*/
	GridIndex getSpatialIndex(Vector transform, Vector globalPos);

	T getSpatial(Vector transform, Vector globalPos);

	//Iterator functions to access the grid
	GridIndex begin();
	GridIndex next(GridIndex index);
	GridIndex end();
	
};

template<class T>
Grid<T>::Grid(float _width, float _height, int _rows, int _cols)
{
	width = _width;
	height = _height;
	
	rows = _rows;
	cols = _cols;

	rowSize = width / cols;
	colSize = height / rows;
}

template<class T>
Grid<T>::~Grid()
{
}

template<class T>
inline void Grid<T>::initialize(T value) {
	for (int i = 0; i < rows; i++) {
		std::vector<T> row;
		for (int j = 0; j < cols; j++) {
			row.push_back(T(value));
		}
		grid.push_back(row);
	}
}

template<class T>
inline void Grid<T>::setPartitionCount(int _rows, int _cols)
{
	rows = _rows;
	cols = _cols;
	rowSize = height / rows;
	colSize = width / cols;
}

template<class T>
inline void Grid<T>::setAll(T value)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid[i][j] = value;
		}
	}
}

template<class T>
inline void Grid<T>::set(int row, int col, T value)
{
	if (row >= 0 && row < rows && col >= 0 && col < cols) {
		grid[row][col] = value;
	}
}

template<class T>
inline void Grid<T>::set(GridIndex index, T value)
{
	if (index.x >= 0 && index.x < cols && index.y >= 0 && index.y < rows) {
		grid[index.y][index.x] = value;
	}
}

template<class T>
inline void Grid<T>::clear(GridIndex index)
{
	grid[index.y][index.x] = NULL;
}

template<class T>
inline T Grid<T>::get(int row, int col)
{
	if (row >= 0 && row < rows && col >= 0 && col < cols) {
		return grid[row][col];
	}
}

template<class T>
inline T Grid<T>::get(GridIndex index)
{
	if (index. y >= 0 && index.y < rows && index.x >= 0 && index.x < cols) {
		return grid[index.y][index.x];
	}
}

template<class T>
inline GridIndex Grid<T>::getSpatialIndex(Vector transform, Vector globalPos)
{
	GridIndex index;
	Vector gridPos = globalPos - transform;
	index.x = floor(gridPos.x / colSize);
	index.y = floor(gridPos.y / rowSize);
	return index;
}

template<class T>
inline T Grid<T>::getSpatial(Vector transform, Vector globalPos)
{
	GridIndex index = getSpatialIndex(transform, globalPos);
	if (index.x >= 0 && index.x < cols && index.y >= 0 && index.y < rows) {
		return grid[index.y][index.x];
	}
	else {
		return NULL;
	}
}

template<class T>
inline GridIndex Grid<T>::begin()
{
	return GridIndex{ 0 ,0 };
}

template<class T>
inline GridIndex Grid<T>::next(GridIndex index)
{
	if (index != end()) {
		if (index.x >= cols - 1) {
			index.y += 1;
			index.x = 0;
		}
		else {
			index.x += 1;
		}
		return index;
	}
	else {
		return begin();
	}
}

template<class T>
inline GridIndex Grid<T>::end()
{
	return GridIndex{ cols - 1, rows - 1 };
}
