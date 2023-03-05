#include "Circle.h"
#include "surface.h"


bool Circle::Collides(const Circle& circle) const
{
	const float r = radius + circle.radius;
	const Tmpl8::vec2 dist = center - circle.center;

	//If R^2 > C^2: return TRUE
	return (r * r) > dist.sqrLentgh(); // Faster than sqrt()
}

