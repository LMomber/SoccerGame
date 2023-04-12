#include "Player.h"

//Player 1
Player::Player(const int a, const int d, const int w, const int space, const Controller& controller) :
	leftKey{ a },
	rightKey{ d },
	upKey1{ w },
	upKey2{ space },
	controller{ controller }
{}

//Player 2
Player::Player(const int leftKey, const int rightKey, const int upKey, const Controller& controller) :
	leftKey{ leftKey },
	rightKey{ rightKey },
	upKey1{ upKey },
	controller{ controller }
{}

void Player::Move()
{
	const float velocity_X = 500.0f;
	const float velocity_Y = 3000;


	//When movement key pressed:
	if ((controller.GetThumbStickL() < 0) || GetAsyncKeyState(leftKey))
	{
		velocity = { -velocity_X,velocity.y };	//Go left
	}
	else if ((controller.GetThumbStickL() > 0) || GetAsyncKeyState(rightKey))
	{
		velocity = { velocity_X,velocity.y };	//Go right
	}
	else velocity = { 0,velocity.y };			//Stand still

	if ((controller.GetButtonA() || GetAsyncKeyState(upKey1) || GetAsyncKeyState(upKey2)) && (rectangle.pos.y == bottomBorder))
	{
		velocity = { velocity.x,-velocity_Y };	 //Jump
	}
}
