#include "Circle.h"
#include "surface.h"

//Circle-circle collision
bool Circle::Collides(const Circle& circle) const
{
	const int r = radius + circle.radius;
	const Tmpl8::vec2 dist = center - circle.center;

	//If the radius > the distance between the circles: return true
	return (r * r) > dist.sqrLentgh();
}