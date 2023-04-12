#pragma once

#include "template.h"

/************************************
	Structure for making a circle.
*************************************/

struct Circle
{
	Tmpl8::vec2 center{ 0 };
	int radius{ 0 };


	//Circle-circle collision
	bool Collides(const Circle& circle) const;

};