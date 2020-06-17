#pragma once
#include "Vector.h"
#include <vector>
#include <SDL_stdinc.h>
#include "Display.h"
#include "HaGame2D.h"

class MeshRenderer : public Component {

	std::vector<Triangle> tris;

public:
	MeshRenderer(std::vector<Triangle> triangles) {
		tris = triangles;
	}

	void update() {

		transform->rotate(1 * M_PI / 180 * (gameScene->dt() / 5000));
		transform->setOrigin(Vector(sin(transform->rotation) * 50, sin(transform->rotation) * 50));

		float vel = 0.001;

		if (input->up) {
			transform->move(Vector(0, -1 * vel * gameScene->dt(), 0));
		}

		if (input->down) {
			transform->move(Vector(0, vel * gameScene->dt(), 0));
		}

		if (input->right) {
			transform->move(Vector(vel * gameScene->dt(), 0, 0));
		}

		if (input->left) {
			transform->move(Vector(-1 * vel * gameScene->dt(), 0, 0));
		}

		if (input->fire1) {
			transform->move(Vector(0, 0, 1 * vel * 0.1 * gameScene->dt()));
		}

		if (input->fire2) {
			transform->move(Vector(0, 0, -1 * vel * 0.1 * gameScene->dt()));
		}

		for (auto tri : tris) {
			tri.p1 = tri.p1 * 50;
			tri.p2 = tri.p2 * 50;
			tri.p3 = tri.p3 * 50;

			Triangle adjusted = Triangle{
				transform->position + Math::rotatePointAroundPoint(tri.p1, transform->origin, transform->rotation),
				transform->position + Math::rotatePointAroundPoint(tri.p2, transform->origin, transform->rotation),
				transform->position + Math::rotatePointAroundPoint(tri.p3, transform->origin, transform->rotation)
			};


			/*Triangle adjusted = Triangle{
				transform->position + tri.p1,
				transform->position + tri.p2,
				transform->position + tri.p3
			};
			*/
			//std::cout << adjusted.toString() << std::endl;

			//std::cout << adjusted.toString() << std::endl;

			display->drawTriangle(adjusted, Color::blue());
		}
	}
};


struct Mesh {
	std::vector<Triangle> tris;

	static Mesh get3DCube(Vector size, Vector position) {
		Mesh mesh;
		mesh.tris = {
			//Front face
			Triangle{ Vector(0, 0, 0), Vector(size.x, 0, 0), Vector(size.x, size.y, 0) },
			Triangle{ Vector(0, 0, 0), Vector(size.x, size.y, 0), Vector(0, size.y, 0) },

			//Back Face
			Triangle{ Vector(0, 0, size.z), Vector(size.x, 0, size.z), size },
			Triangle{ Vector(0, 0, size.z), size, Vector(0, size.y, size.z) },

			//North Face
			Triangle{ Vector(0, 0, 0), Vector(0, 0, size.z), Vector(size.x, 0, size.x) },
			Triangle{ Vector(0, 0, 0), Vector(size.x, 0, size.z), Vector(size.x, 0, 0) },

			//East Face
			Triangle{ Vector(size.x, 0, 0), Vector(size.x, 0, size.z), size },
			Triangle{ Vector(size.x, 0, 0), size, Vector(size.x, size.y, 0) },

			//South Face
			Triangle{ Vector(0, size.y, 0), Vector(0, size.y, size.z), size},
			Triangle{ Vector(0, size.y, 0), size, Vector(size.x, size.y, 0) },

			//West Face
			Triangle{ Vector(0, 0, size.z), Vector(0, 0, 0), Vector(0, size.y, 0) },
			Triangle{ Vector(0, 0, size.z), Vector(0, size.y, 0), Vector(0, size.y, size.z) },
		};

		for (auto tri : mesh.tris) {
			tri.p1 += position;
			tri.p2 += position;
			tri.p3 += position;
		}

		return mesh;
	}

};

struct Vector4 {
	float x, y, z, w;

	Vector toVector() {
		return Vector{ x, y, z };
	}

	static Vector4 toVector4(Vector v, float _w) {
		return Vector4{ v.x, v.y, v.z, _w };
	}
};

struct Matrix4 {
	float m[4][4];

	Vector4 leftMult(Vector4 v) {
		return Vector4 {
			v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
			v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
			v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
			v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3],
		};
	}
};

class Display3D
{
public:
	static Matrix4 projectionMatrix(float fov, float nearClip, float farClip) {
		float S = 1 / tan((fov / 2) * (M_PI / 180));
		float f1 = (-1 * farClip) / (farClip - nearClip);
		float f2 = (-1 * farClip * nearClip) / (farClip - nearClip);

		// std::cout << "F1 = " << f1 << " F2 = " << f2 << std::endl << " S = " << S;

		return Matrix4{
			{
				{S, 0, 0, 0},
				{0, S, 0, 0},
				{0, 0, f1, -1},
				{0, 0, f2, 0}
			}
		};
	}

	static Vector projectPoint(Vector point, float fov, float nearClip, float farClip) {
		Matrix4 projMatrix = projectionMatrix(fov, nearClip, farClip);
		Vector4 projected = projMatrix.leftMult(Vector4::toVector4(point, 1));
		// std::cout << point.toString() << std::endl;
		// std::cout << projected.x << ", " << projected.y << ", " << projected.z << ", " << projected.w << std::endl;

		Vector out = projected.toVector();

		if (projected.w != 0) {
			out = out * (1 / projected.w);
		}

		return out;
	}
};

