#include "Player.h"

Player::Player()
{}

void Player::Move(Ball& ball, int left, int right, int jump, Controller& controller, float thumbStickL, bool a_Button)
{
	float velocity_X = 500.0f;
	float velocity_Y = 3000;

	WallCollision();

	BallCollision(ball);

	if (controller.IsConnected())
	{
		if (thumbStickL < 0)
		{
			velocity = { -velocity_X,velocity.y };
		}
		else if (thumbStickL > 0)
		{
			velocity = { velocity_X,velocity.y };
		}
		else velocity = { 0,velocity.y };

		if (rectangle.bottomCenter.y == (ScreenHeight - 2) && a_Button)
		{
			velocity = { velocity.x,-velocity_Y };
		}
	}
	else 
	{
		//When movement key pressed:
		if (GetAsyncKeyState(left))
		{
			velocity = { -velocity_X,velocity.y };
		}
		else if (GetAsyncKeyState(right))
		{
			velocity = { velocity_X,velocity.y };
		}
		else velocity = { 0,velocity.y };

		if (rectangle.bottomCenter.y == (ScreenHeight - 2) && GetAsyncKeyState(jump) & 1)
		{
			velocity = { velocity.x,-velocity_Y };
		}
	}
}