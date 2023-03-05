#pragma once

#include "template.h"

namespace Shape
{
	struct Rectangle
	{
		int width;
		int height;
		Tmpl8::vec2 bottomCenter{ 0 };
	};
}