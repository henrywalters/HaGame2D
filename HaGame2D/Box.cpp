#include "stdafx.h"
#include "Box.h"
#include "Circle.h"
#include "Vector.h"

bool Box::collidingWithCircle(Circle circle)
{

	Vector closestPoint = Vector(
		Math::clamp(circle.center.x, x, x + width),
		Math::clamp(circle.center.y, y, y + height)
	);

	Vector distance = circle.center - closestPoint;

	return distance.magnitude() <= circle.radius;
}
