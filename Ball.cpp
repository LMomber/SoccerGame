#include "Ball.h"

Ball::Ball()
	: circleCenter{ {0}, 25 }
{}

void Ball::Update(float deltaTime)
{
    velocity += gravity;
    circleCenter.center += velocity * deltaTime;
}

void Ball::Draw(Tmpl8::Surface* screen)
{
	int center_X = static_cast<int>(circleCenter.center.x);
	int center_Y = static_cast<int>(circleCenter.center.y);

	for (int i = (center_X - circleCenter.radius); i < (center_X + circleCenter.radius); i++) //for x
	{
		int C = circleCenter.radius;
		int A = (i - center_X);
		float B = static_cast<float>(sqrt(C * C - A * A)); //B^2 = C^2 - A^2

		screen->Line(circleCenter.center.x + A, circleCenter.center.y + B, circleCenter.center.x + A, circleCenter.center.y, color);
		screen->Line(circleCenter.center.x + A, circleCenter.center.y - B, circleCenter.center.x + A, circleCenter.center.y, color);
	}

	for (int i = (center_Y - circleCenter.radius); i < (center_Y + circleCenter.radius); i++) //for y
	{
		int C = circleCenter.radius;
		int B = (i - center_Y);
		float A = static_cast<float>(sqrt(C * C - B * B)); //A^2 = C^2 - B^2

		screen->Line(circleCenter.center.x + A, circleCenter.center.y + B, circleCenter.center.x, circleCenter.center.y + B, color);
		screen->Line(circleCenter.center.x - A, circleCenter.center.y + B, circleCenter.center.x, circleCenter.center.y + B, color);
	}
}

const Tmpl8::vec2& Ball::GetPosition() const
{
	return circleCenter.center;
}

void Ball::SetPosition(Tmpl8::vec2 pos)
{
	circleCenter.center = pos;
}

const int Ball::GetRadius() const
{
	return circleCenter.radius;
}

void Ball::SetRadius(int radius)
{
	circleCenter.radius = radius;
}

const Circle& Ball::GetCircle() const
{
	return circleCenter;
}

void Ball::WallCollision()
{
	float maxVelocity = -1000.0f;
	float radius = static_cast<float>(circleCenter.radius);

	if (velocity.y <= maxVelocity)
	{
		velocity.y = maxVelocity;
	}
	//Hit left side of screen.
	if (circleCenter.center.x - radius <= 0.0f)
	{
		circleCenter.center.x = radius;
		velocity.x = -velocity.x;
	}
	//Hit right side of screen.
	else if (circleCenter.center.x + radius >= ScreenWidth)
	{
		circleCenter.center.x = ScreenWidth - radius;
		velocity.x = -velocity.x;
	}
	//Hit Top of screen
	if (circleCenter.center.y - radius <= 0.0f)
	{
		circleCenter.center.y = radius;
		velocity.y = -velocity.y;
	}
	//Hit Bottom of screen
	else if (circleCenter.center.y + radius >= ScreenHeight)
	{
		circleCenter.center.y = ScreenHeight - radius;
		velocity.y = -velocity.y - 100;
	}
}
