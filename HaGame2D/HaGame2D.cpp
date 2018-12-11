// HaGame2D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SDL.h"
#include "HaGame2D.h"
#include <iostream>
//#include "Pong.h"
#include "ZeldaClone.h"
#include "SpriteSheetHelper.h"
//#include "SpriteSheetLoader.h"
//#include "CyclicList.h"
#include "MapBuilder.h"
//#include "HaType.h"
//#include "ParameterSet.h"
#include "MapLoader.h"

int main(int argc, char* argv[])
{

	//ParameterSetList list;
	//list.load("maptest.txt");

	//auto set = list.getSet(0);
	
	//float x = set.get<float>("x");

	//std::cout << x << "\n"
	

	//Pong pong;
	
	ZeldaClone zelda = ZeldaClone();

    //SpriteSheetHelper helper = SpriteSheetHelper(800, 100);

	//MapBuilder builder("demo.txt");

	//HaType hatype;
	/*
	

	float p[] = { 1, 1, 2, 2 };
	float v[] = { 1, 1, 4, 4 };
	float scale = 2.0;

	Matrix P = Matrix(p);
	Matrix V = Matrix(v);

	Matrix Pv = P - Matrix::translationMatrix(V);
	Matrix Ps = Matrix::scaleMatrix(scale) * Pv;

	std::cout << P.toString() << "Mapped to: " << Ps.toString();

	Matrix PvP = Matrix::scaleMatrix(1 / scale) * Ps;
	Matrix PP = Matrix::translationMatrix(V) + PvP;

	std::cout << "Inverse mapping\n";
	std::cout << Ps.toString() << "Mapped to: " << PP.toString();

	V = V - Matrix::translationMatrix(0.5 * scale) + Matrix::sizeMatrix(scale);

	std::cout << "Scaled Viewport: " << V.toString();

	Pv = P - Matrix::translationMatrix(V);
	Ps = Matrix::scaleMatrix(scale) * Pv;

	std::cout << P.toString() << "Mapped to: " << Ps.toString();

	PvP = Matrix::scaleMatrix(1 / scale) * Ps;
	PP = Matrix::translationMatrix(V) + PvP;

	std::cout << "Inverse mapping\n";
	std::cout << Ps.toString() << "Mapped to: " << PP.toString();


	*/
	return 0;
}
