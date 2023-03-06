#include "Opponent.h"

Opponent::Opponent()
{
}

void Opponent::Move(Ball& ball)
{
	float velocity_X = 500.0f;
	float velocity_Y = 3000;

	int distance_Behind_Ball = 50;

	int high_Ball = 200;
	int low_Ball = 250;

	int AI_x = rectangle.bottomCenter.x;
	int AI_y = rectangle.bottomCenter.y;

	WallCollision();

	BallCollision(ball);

	//If high ball:
		//If in front of AI:
			//If ball moving forwards:
				//If ball is far away && AI is NOT at mid_distance (or further):
					//Move left
			//Else If ball moves backwards && AI is NOT at keepers_distance (or further):
				//Move right
			//Else If ball X = 0 && AI is NOT at jump_distance (or further):
				//Move left
			//Else
				//Wait
		//If behind AI:
			//If ball moving forwards || ball has X = 0 || (ball moves backwards && AI is NOT at keepers_distance (or further)):
				//Move right
			//Else
				//Wait

	//If jump ball:
		//If in front of AI:
			//If ball moving forwards:
				//If (ball is far away || (!(ball is far away) && ball is moving mid/fast)):
					//If AI is at mid_distance (or further):
						//Wait
					//Else
						//Move left
				//Else If AI is very close to the ball:
					//Jump to the left
				//Else
					//Move left
			//If ball moves backwards:
				//If ball is far away:
					//If AI is at goal (or further):
						//Wait
					//Else
						//Move right
				//Else
					//Jump to the left
			//If ball X = 0:
				//If AI is at jump_distance (or further):
					//Jump to the left
				//Else
					//Move left
		//If behind AI:
			//Move right
	
	//If low ball:
		//If in front of AI:
			//If ball is moving forward:
				//If ball is far away && AI is at mid_distane (or further):
					//Wait
				//Else
					//Move left
			//If ball moves backwards:
				//If the ball is far away:
					//Move right
				//Else If the ball is very close:
					//Move left
				//Else
					//Wait
			//If ball X = 0:
				//If AI is at mid_distance (or further):
					//Wait
				//Else
					//Move left
		//If behind AI:
			//If ball is moving forwards:
				//If ball is far away:
					//Move right
				//Else If ball is very close:
					//If ball is at head height:
						//If the ball moves up:
							//Wait
						//Else
							//Move left
					//Else
						//Move left
				//Else
					//Move Right
			//If ball moves backwards:
				//If AI is just in front of ball:
					//Wait
				//Else
					//Move right
			//If ball X = 0:
				//If ball moves slower than AI:
					//If the AI is very near the ball:
						//If the ball is higher than the AI:
							//Move right
						//Else
							//Wait
					//Else
						//Move right
				//Else 
					//Move right


	//BALL IS HIGH:
	if (ball.GetPosition().y < high_Ball)
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
