#pragma once

#include "template.h"

struct Circle
{
	Tmpl8::vec2 center{ 0 };
	int radius{ 0 };

	bool Collides(const Circle& circle) const;

};