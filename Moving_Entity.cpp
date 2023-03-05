#include "Moving_Entity.h"

const Tmpl8::vec2& Moving_Entity::GetVelocity() const
{
    return velocity;
}

void Moving_Entity::SetVelocity(Tmpl8::vec2 vel)
{
    velocity = vel;
}

const Tmpl8::vec2& Moving_Entity::GetGravity() const
{
    return gravity;
}

void Moving_Entity::SetGravity(Tmpl8::vec2 grav)
{
    gravity = grav;
}

const Tmpl8::Pixel Moving_Entity::GetColor() const
{
    return Tmpl8::Pixel();
}

void Moving_Entity::SetColor(Tmpl8::Pixel color)
{
    this->color = color;
}

void Moving_Entity::WallCollision()
{
	int x{ 0 };
	int y{ 0 };
	int radius{ 0 };

	//Hit left side of screen.
	if (x - radius <= 0.0f)
	{
		x = radius;
		velocity.x = -velocity.x;
	}
	//Hit right side of screen.
	else if (x + radius >= ScreenWidth)
	{
		x = ScreenWidth - radius;
		velocity.x = -velocity.x;
	}
	//Hit Top of screen
	if (y - radius <= 0.0f)
	{
		y = radius;
		velocity.y = -velocity.y;
	}
	//Hit Bottom of screen
	else if (y + radius >= ScreenHeight)
	{
		y = ScreenHeight - radius;
		velocity.y = -velocity.y - 100;
	}
}
