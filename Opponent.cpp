#include "Opponent.h"

Opponent::Opponent()
{
}

void Opponent::Move(Ball& ball)
{
	float velocity_X = 500.0f;
	float velocity_Y = 3000;

	int AI_x = rectangle.bottomCenter.x;
	int AI_y = rectangle.bottomCenter.y;

	int high_Ball = 200;
	int low_Ball = 350;

	int goal_Pos = ScreenWidth - 100;
	int mid_Pos = 500;
	int keeper_Pos = 800;

	int jump_Distance = AI_x - (rectangle.width/2) - 100;
	int far_Away = 600;

	int fast_Ball = 1000;
	int mid_Ball = 400;
	int slow_Ball = 300;

	unsigned int ball_Distance = (AI_x > ball.GetPosition().x) ? AI_x - ball.GetPosition().x : ball.GetPosition().x - AI_x;

	WallCollision();

	BallCollision(ball);
	/*
	If high ball:
		If in front of AI:
			If ball moving forwards:
				If ball is far away && AI is NOT at mid_distance (or further):
					Move left
			Else If ball moves backwards && AI is NOT at keepers_distance (or further):
				Move right
			Else If ball X = 0 && AI is NOT at jump_distance (or further):
				Move left
			Else
				Wait
		If behind AI:
			If ball moving forwards || ball has X = 0 || (ball moves backwards && AI is NOT at keepers_distance (or further)):
				Move right
			Else
				Wait

	If jump ball:
		If in front of AI:
			If ball moving forwards:
				If (ball is far away || (!(ball is far away) && ball is moving mid/fast)):
					If AI is at mid_distance (or further):
						Wait
					Else
						Move left
				Else If AI is very close to the ball:
					Jump to the left
				Else
					Move left
			If ball moves backwards:
				If ball is far away:
					If AI is at goal (or further):
						Wait
					Else
						Move right
				Else
					If ball is pretty close to AI (or closer):
						Jump to the left
					Else
						If AI is at mid_distance:
							Wait
						Else
							Move left
			If ball X = 0:
				If AI is at jump_distance (or further):
					Jump to the left
				Else
					Move left
		If behind AI:
			Move right
	
	If low ball:
		If in front of AI:
			If ball is moving forward:
				If ball is far away && AI is at mid_distane (or further):
					Wait
				Else
					Move left
			If ball moves backwards:
				If the ball is far away:
					Move right
				Else If the ball is very close:
					Move left
				Else
					Wait
			If ball X = 0:
				If AI is at mid_distance (or further):
					Wait
				Else
					Move left
		If behind AI:
			If ball is moving forwards:
				If ball is far away:
					Move right
				Else If ball is pretty close:
					If ball is at head height:
						If the ball moves up:
							Wait
						Else
							Move left
					Else
						Move left
				Else
					Move Right
			If ball moves backwards:
				If AI is very close to the ball:
					Wait
				Else
					Move right
			If ball X = 0:
				If ball moves slower than AI:
					If the AI is very near the ball:
						If the ball is higher than the AI:
							Move right
						Else
							Wait
					Else
						Move right
				Else 
					Move right
	*/

	//HIGH BALL
	if (ball.GetPosition().y < high_Ball)
	{
		//Ball in front of AI
		if (ball.GetPosition().x < AI_x)
		{
			//Ball moving forward
			if (ball.GetVelocity().x < 0)
			{
				//Ball is far away && AI is NOT at mid_Pos or further
				if ((ball_Distance >= far_Away) && (AI_x >= mid_Pos))
				{
					velocity = { -velocity_X,velocity.y }; //Move left
				}
			}
			//Ball moving backwards && AI is NOT at keeper_Pos or further
			else if ((ball.GetVelocity().x > 0) && (AI_x < keeper_Pos))
			{
				velocity = { velocity_X,velocity.y }; //Move right
			}
			//Ball X = 0 && AI is NOT at jump_Distance
			else if ((ball.GetVelocity().x == 0) && (ball_Distance > jump_Distance))
			{
				velocity = { -velocity_X,velocity.y }; //Move left
			}
			else velocity = { 0,velocity.y }; //Wait
		}

		//Ball behind AI
		if (ball.GetPosition().x > AI_x)
		{
			//ball moving forward || ball X = 0 || (ball moves backwards && AI is NOT at keeper_Pos or further)
			if ((ball.GetVelocity().x < 0) || (ball.GetVelocity().x == 0) || (ball.GetVelocity().x > 0) && (AI_x < keeper_Pos))
			{
				velocity = { velocity_X,velocity.y }; //Move right
			}
			else velocity = { 0,velocity.y }; //Wait
		}
	}


	//JUMP BALL
	else if ((ball.GetPosition().y >= high_Ball) && (ball.GetPosition().y <= low_Ball))
	{
		//Ball in front of AI
		if (ball.GetPosition().x < AI_x)
		{
			//Ball moving forward
			if (ball.GetVelocity().x < 0)
			{
				//If (ball is far away || (!(ball is far away) && ball is moving mid/fast))
				if ((ball_Distance >= far_Away) || (!(ball_Distance >= far_Away) && ball.GetVelocity().x >= mid_Ball))
				{
					//If AI is at mid_distance or further
					if (AI_x <= mid_Pos)
					{
						velocity = { 0,velocity.y }; //Wait
					}
					else velocity = { -velocity_X,velocity.y }; //Move left
				}
				//If AI is very close to the ball
				else if (ball_Distance <= jump_Distance)
				{
					//Jump to the left
					velocity = { -velocity_X,velocity.y };
					if (rectangle.bottomCenter.y == (ScreenHeight - 2))
					{
						velocity = { velocity.x,-velocity_Y };
					}
				}
				else velocity = { -velocity_X,velocity.y }; //Move left
			}

			//Ball moving backwards
			if (ball.GetVelocity().x > 0)
			{
				//If ball is far away
				if (ball_Distance >= far_Away)
				{
					//If AI is at goal or further
					if (AI_x >= goal_Pos)
					{
						velocity = { 0,velocity.y }; //Wait
					}
					else velocity = { velocity_X,velocity.y }; //Move right
				}
				else
				{
					//If ball is pretty close to AI or closer
					if (ball_Distance <= (jump_Distance + 50))
					{ 
						//Jump to the left
						velocity = { -velocity_X,velocity.y };
						if (rectangle.bottomCenter.y == (ScreenHeight - 2))
						{
							velocity = { velocity.x,-velocity_Y };
						}
					}
					else
					{
						//If AI is at mid_distance or further
						if (AI_x <= mid_Pos)
						{
							velocity = { 0,velocity.y }; //Wait
						}
						else velocity = { -velocity_X,velocity.y }; //Move left
					}
				}
			}

			//If ball X = 0
			if (ball.GetVelocity().x == 0)
			{
				//If AI is at jump_Distance or further
				if (ball_Distance <= jump_Distance)
				{
					//Jump to the left
					velocity = { -velocity_X,velocity.y };
					if (rectangle.bottomCenter.y == (ScreenHeight - 2))
					{
						velocity = { velocity.x,-velocity_Y };
					}
				}
				else velocity = { -velocity_X,velocity.y }; //Move left
			}
		}

		//If behind AI:
		if (ball.GetPosition().x > AI_x)
		{
			velocity = { velocity_X,velocity.y }; //Move right
		}
	}


	//LOW BALL
	else if (ball.GetPosition().y >= low_Ball)
	{
		//Ball in front of AI
		if (ball.GetPosition().x < AI_x)
		{
			//Ball moving forward
			if (ball.GetVelocity().x < 0)
			{
				//If ball is far away && AI is at mid_Pos or further
				if ((ball_Distance >= far_Away) && (AI_x <= mid_Pos))
				{
					velocity = { 0,velocity.y }; //Wait
				}
				else velocity = { -velocity_X,velocity.y }; //Move left
			}

			//Ball moving backwards
			if (ball.GetVelocity().x > 0)
			{
				//If ball is far away
				if (ball_Distance >= far_Away)
				{
					velocity = { velocity_X,velocity.y }; //Move right
				}
				//If the ball is very close
				else if (ball_Distance <= jump_Distance)
				{
					velocity = { -velocity_X,velocity.y }; //Move left
				}
				else velocity = { 0,velocity.y }; //Wait
			}

			//If ball X = 0
			if (ball.GetVelocity().x == 0)
			{
				//If AI is at mid_Pos or further
				if (AI_x <= mid_Pos)
				{
					velocity = { 0,velocity.y }; //Wait
				}
				else velocity = { -velocity_X,velocity.y }; //Move left
			}
		}

		//If behind AI:
		if (ball.GetPosition().x > AI_x)
		{
			//Ball moving forward
			if (ball.GetVelocity().x < 0)
			{
				//If ball is far away
				if (ball_Distance >= far_Away)
				{
					velocity = { velocity_X,velocity.y }; //Move right
				}
				//If ball is pretty close to AI or closer
				else if (ball_Distance <= (jump_Distance + 50))
				{
					//If Ball is at head height
					if (ball.GetPosition().y <= 425)
					{
						if (ball.GetVelocity().y < 0)
						{
							velocity = { 0,velocity.y }; //Wait
						}
						else velocity = { -velocity_X,velocity.y }; //Move left
					}
					else velocity = { -velocity_X,velocity.y }; //Move left
				}
				else velocity = { velocity_X,velocity.y }; //Move right
			}

			//Ball moving backwards
			if (ball.GetVelocity().x > 0)
			{
				//If the ball is very close
				if (ball_Distance <= jump_Distance)
				{
					velocity = { 0,velocity.y }; //Wait
				}
				else velocity = { velocity_X,velocity.y }; //Move right
			}

			//If ball X = 0
			if (ball.GetVelocity().x == 0)
			{
				//If ball is slower than AI
				if (ball.GetVelocity().x <= slow_Ball)
				{
					//If the ball is very close
					if (ball_Distance <= jump_Distance)
					{
						//If Ball is higher than the AI
						if (ball.GetPosition().y < (ScreenHeight - 85))
						{
							velocity = { velocity_X,velocity.y }; //Move right
						}
						else velocity = { 0,velocity.y }; //Wait
					}
					else velocity = { velocity_X,velocity.y }; //Move right
				}
				else velocity = { velocity_X,velocity.y }; //Move right
			}
		}
	}
}
