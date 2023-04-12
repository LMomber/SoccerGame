#pragma once

#include "template.h"

/************************************
	Structure for making a rectangle.
*************************************/

namespace Shape
{
	struct Rectangle
	{
		int width;
		int height;
		Tmpl8::vec2 pos{ 0 };
	};
}