#include "Opponent.h"

Opponent::Opponent()
{
}

void Opponent::Move(Ball& ball)
{
	float velocity_X = 500.0f;
	float velocity_Y = 3000;

	int distance_Behind_Ball = 50;

	int height_Until_Jump = 200;
	int low_Ball = 250;

	int AI_x = rectangle.bottomCenter.x;
	int AI_y = rectangle.bottomCenter.y;

	WallCollision();

	BallCollision(ball);

	//BALL IS HIGH:
	if (ball.GetPosition().y < height_Until_Jump)
	{
		if ((ball.GetPosition().x < AI_x - distance_Behind_Ball) && ball.GetVelocity().x < 0) //If AI is behind the ball & the ball is going forward:
		{
			if ((AI_x - ball.GetPosition().x) > 300) //If distance between AI and ball is bigger than 300:
			{
				velocity = { 0, velocity.y }; //Stand still.
			}
			else velocity = { -velocity_X,velocity.y }; //Move left.
		}

		/*if ((ball.GetPosition().x < AI_x + distance_Behind_Ball) && ball.GetVelocity().x > 0) //If AI is behind the ball & the ball is going backwards:
		{
			if ((AI_x - ball.GetPosition().x) < 300) //If distance between AI and ball is bigger than 300:
			{
				velocity = { 0, velocity.y }; //Stand still.
			}
			else velocity = { velocity_X,velocity.y }; //Move right.
		}

		if ((ball.GetPosition().x > AI_x + distance_Behind_Ball) && ball.GetVelocity().x < 0) //If AI is in front of the ball & the ball is going forward:
		{
			if ((ball.GetPosition().x - AI_x) > 100) //If distance between AI and ball is bigger than 100:
			{
				velocity = { velocity_X,velocity.y }; //Move right.
			}
			else velocity = { 0, velocity.y }; //Stand still.
		}

		if ((ball.GetPosition().x > AI_x + distance_Behind_Ball) && ball.GetVelocity().x > 0) //If AI is in front of the ball & the ball is going backwards:
		{
			velocity = { velocity_X,velocity.y }; //Move right.
		}
	}

	//ON JUMP HEIGHT
	if ((ball.GetPosition().y >= height_Until_Jump) && (ball.GetPosition().y <= low_Ball)) //If ball is on jump height:
	{
		if ((ball.GetPosition().x < AI_x + distance_Behind_Ball) && ball.GetVelocity().x < 0) //If AI is behind the ball & the ball is going forward:
		{
			if ((AI_x - ball.GetPosition().x) > 300) //If distance between AI and ball is bigger than 300:
			{
				velocity = { 0, velocity.y }; //Stand still.
			}
			else velocity = { -velocity_X,velocity.y }; //Move left.
		}

		if ((ball.GetPosition().x < AI_x + distance_Behind_Ball) && ball.GetVelocity().x > 0) //If AI is behind the ball & the ball is going backwards:
		{
			if ((AI_x - ball.GetPosition().x) < 300) //If distance between AI and ball is bigger than 300:
			{
				velocity = { 0, velocity.y }; //Stand still.
			}
			else velocity = { velocity_X,velocity.y }; //Move right.
		}

		if ((ball.GetPosition().x > AI_x + distance_Behind_Ball) && ball.GetVelocity().x < 0) //If AI is in front of the ball & the ball is going forward:
		{
			if ((ball.GetPosition().x - AI_x) > 100) //If distance between AI and ball is bigger than 100:
			{
				velocity = { velocity_X,velocity.y }; //Move right.
			}
			else velocity = { 0, velocity.y }; //Stand still.
		}

		if ((ball.GetPosition().x > AI_x + distance_Behind_Ball) && ball.GetVelocity().x > 0) //If AI is in front of the ball & the ball is going backwards:
		{
			velocity = { velocity_X,velocity.y }; //Move right.
		}
	}

	//BALL IS LOW

	if (ball.GetPosition().x < AI_x)
	{
		velocity = { -velocity_X,velocity.y }; //LEFT

		if (AI_y == (ScreenHeight - 2) && (ball.GetPosition().y > 300 && ball.GetPosition().y < 350))
		{
			velocity = { velocity.x,-velocity_Y };
		}
	}
	else if (ball.GetPosition().x > AI_x && ball.GetVelocity().x > 0)
	{
		velocity = { velocity_X,velocity.y }; //RIGHT
	}
	else
	{
		velocity = { 0,velocity.y }; // Dont move x position
	}*/
	}
}
