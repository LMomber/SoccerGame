#pragma once

#include "template.h"
#include "surface.h"
#include "Circle.h"

class Moving_Entity
{
public:
	const Tmpl8::vec2& GetVelocity() const;
	void SetVelocity(Tmpl8::vec2 vel);

	const Tmpl8::vec2& GetGravity() const;
	void SetGravity(Tmpl8::vec2 grav);

	const Tmpl8::Pixel GetColor() const;
	void SetColor(Tmpl8::Pixel color);

	void WallCollision();

protected:
	Tmpl8::vec2 velocity{ 0 };
	Tmpl8::vec2 gravity{ 0 };
	Tmpl8::Pixel color = Tmpl8::GreenMask;
};

