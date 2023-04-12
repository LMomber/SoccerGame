#include "Ball.h"

Ball::Ball()
	: circle{ {0}, 25 }
{}

void Ball::Update(float deltaTime)
{
	//Physics
	velocity += gravity * (100 * deltaTime);
    circle.center += velocity * deltaTime;

	//Cap the maximal X-velocity to maxVelocity.x
	if (velocity.x <= -maxVelocity.x) velocity.x = -maxVelocity.x;

	//Cap the maximal Y-velocity to maxVelocity.y
	if (velocity.y <= maxVelocity.y) velocity.y = maxVelocity.y;
}

//Resets gravity, velocity, position & frame
void Ball::Reset(Tmpl8::Sprite& sprite)
{
	gravity = { 0, 17 };
	velocity = { 0 };
	circle.center = { ScreenWidth / 2.0f, ScreenHeight / 2.0f };
	sprite.SetFrame(0);
}

//Sets the sprite frame, depending on the velocity of the ball
void Ball::Frames(float deltaTime, Tmpl8::Sprite& sprite)
{
	frame = static_cast<int>(frameFloat);

	/*********************************************************************************

			Makes the sprites switch faster/slower (ball spins faster/slower),
					dependings on the X-velocity of the ball.

			Since the ball switches between only 2 sprites, and frameFloat
			gets truncated, the max value of frameFloat is 1.9 (1 when truncated).


	**********************************************************************************/

	//When the ball moves right:
	if (velocity.x > 0)
	{
		if (frameFloat < 1.9)
		{
			if (velocity.x >= 600)
			{
				frameFloat += 40 * deltaTime;
			}
			else if (velocity.x >= 550)
			{
				frameFloat += 25 * deltaTime;
			}
			else if (velocity.x >= 200)
			{
				frameFloat += 15 * deltaTime;
			}
			else if (velocity.x >= 100)
			{
				frameFloat += 10 * deltaTime;
			}
			else if (velocity.x < 100)
			{
				frameFloat += 5 * deltaTime;
			}
		}
		else if (frameFloat >= 1.9)
		{
			frameFloat = 0;
		}
	}

	//When the ball moves left:
	if (velocity.x < 0)
	{
		if (frameFloat > 0.9)
		{
			if (-velocity.x >= 600)
			{
				frameFloat -= 40 * deltaTime;
			}
			else if (velocity.x >= 550)
			{
				frameFloat += 25 * deltaTime;
			}
			else if (velocity.x >= 200)
			{
				frameFloat += 15 * deltaTime;
			}
			else if (velocity.x >= 100)
			{
				frameFloat -= 10 * deltaTime;
			}
			else if (velocity.x < 100)
			{
				frameFloat -= 5 * deltaTime;
			}
		}
		else if (frameFloat <= 0.9)
		{
			frameFloat = 1.9;
		}
	}

	//When X = 0:
	if (velocity.x == 0) frameFloat = 0;

	sprite.SetFrame(frame);
}

const Tmpl8::vec2& Ball::GetGravity() const
{
	return gravity;
}

const Tmpl8::vec2& Ball::GetVelocity() const
{
	return velocity;
}

const Tmpl8::vec2& Ball::GetPosition() const
{
	return circle.center;
}

void Ball::SetPosition(Tmpl8::vec2 pos)
{
	circle.center = pos;
}

const int Ball::GetRadius() const
{
	return circle.radius;
}

void Ball::SetRadius(int radius)
{
	circle.radius = radius;
}

const Circle& Ball::GetCircle() const
{
	return circle;
}

//Bool to see if the ball touches the bottom border
bool Ball::IsGround() const
{
	//Returns true when the ball hits the bottom border
	if ((circle.center.y + circle.radius) >= bottomBorder + 10)
	{
		return true;
	}
	else return false;
}

void Ball::WallCollision()
{
	//To make the ball go a little bit lower than the grass
	int bottomBorder = this->bottomBorder + 10;

	float radiusFloat = static_cast<float>(circle.radius);

	/****************************************************************************
	 
			When hitting a border, set the corresponding position & velocity.

	*****************************************************************************/

	//Hit left side of screen.
	if (circle.center.x - radiusFloat <= 0.0f)
	{
		circle.center.x = radiusFloat;
		velocity.x = -velocity.x;
	}
	//Hit right side of screen.
	else if (circle.center.x + radiusFloat >= ScreenWidth)
	{
		circle.center.x = ScreenWidth - radiusFloat;
		velocity.x = -velocity.x;
	}
	//Hit Top of screen
	if (circle.center.y - radiusFloat <= 0.0f)
	{
		circle.center.y = radiusFloat;
		velocity.y = -velocity.y;
	}
	//Hit Bottom of screen
	else if (circle.center.y + radiusFloat >= bottomBorder)
	{
		circle.center.y = bottomBorder - radiusFloat;
		velocity.y = -velocity.y;
	}
}