#include "stdafx.h"
#include "Box.h"
#include "Circle.h"
#include "Vector.h"
#include "Line.h"

bool Box::collidingWithCircle(Circle circle)
{

	Vector closestPoint = Vector(
		Math::clamp(circle.center.x, x, x + width),
		Math::clamp(circle.center.y, y, y + height)
	);

	Vector distance = circle.center - closestPoint;

	return distance.magnitude() <= circle.radius;
}

bool Box::collidingWithLine(Line line)
{
	return Algorithms::LiangBarsky(x, y, x + width, y + height, line.p1().x, line.p1().y, line.p2().x, line.p2().y).has_value();
}
