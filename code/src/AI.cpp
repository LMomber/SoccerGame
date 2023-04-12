#include "AI.h"

AI::AI(const int mid_Pos, const int keeper_Pos, const int goal_Pos)
{
	this->mid_Pos = mid_Pos;
	this->keeper_Pos = keeper_Pos;
	this->goal_Pos = goal_Pos;
}

void AI::Move(const Ball& ball)
{
	Tmpl8::vec2 AI_Pos = { rectangle.pos.x, rectangle.pos.y };

	Tmpl8::vec2 ball_Pos = { ball.GetPosition().x, ball.GetPosition().y };
	Tmpl8::vec2 ball_Velocity = { ball.GetVelocity().x, ball.GetVelocity().y };

	//Distance between AI & ball
	float ball_Distance = (AI_Pos.x > ball_Pos.x) ? AI_Pos.x - ball_Pos.x : ball_Pos.x - AI_Pos.x;

	/*********************************************************************************************
	
		This function is divided into 3 parts, based on the Y-position of the ball:

		- High ball, jump ball and low ball.

		The code is explained in detail above the individual lines of code.

		Because the AI is positioned at the right side of the screen, forwards == to the left.

		mid_Pos ~= the middle of the field.
		goal_Pos = inside the AI's goal.
		keeper_Pos = just in front of the AI's goal.

		for a more detailed explanation, see appendix "Flowchart_AI.drawio".
		Draw.io files can be opened on https://www.diagrams.net
	
	**********************************************************************************************/

	//HIGH BALL
	if (ball_Pos.y < high_Ball)
	{
		//Ball in front of AI
		if (ball_Pos.x < AI_Pos.x)
		{
			//Ball moving forward
			if (ball_Velocity.x < 0)
			{
				//Ball is far away && AI is NOT at keeper_Pos or further
				if ((ball_Distance >= far_Away) && (AI_Pos.x >= keeper_Pos))
				{
					velocity = { -AI_Velocity.x,velocity.y }; //Move left
				}
			}
			//Ball moving backwards && AI is NOT at goal_Pos or further
			else if ((ball_Velocity.x > 0) && (AI_Pos.x < goal_Pos))
			{
				velocity = { AI_Velocity.x,velocity.y }; //Move right
			}
			//Ball X = 0 && AI is NOT at jump_Distance
			else if ((ball_Velocity.x == 0) && (ball_Distance > jump_Distance))
			{
				if (AI_Pos.x > mid_Pos) velocity = { -AI_Velocity.x,velocity.y }; //Move left
			}
			else velocity = { 0,velocity.y }; //Wait
		}

		//Ball behind AI
		if (ball_Pos.x > AI_Pos.x)
		{
			//If AI is NOT at goal_Pos or further
			if (AI_Pos.x < goal_Pos)
			{
				velocity = { AI_Velocity.x, velocity.y }; //Move right
			}
			else velocity = { 0, velocity.y }; //Wait
		}
	}

	//JUMP BALL
	else if ((ball_Pos.y >= high_Ball) && (ball_Pos.y <= low_Ball))
	{
		//Ball in front of AI
		if (ball_Pos.x < AI_Pos.x)
		{
			//Ball moving forward
			if (ball_Velocity.x < 0)
			{
				//If ball is far away || ball is Not moving slow
				if ((ball_Distance >= far_Away) || (ball_Velocity.x >= mid_Ball))
				{
					//If AI is at keeper_Pos or further
					if (AI_Pos.x <= keeper_Pos)
					{
						velocity = { 0,velocity.y }; //Wait
					}
					else velocity = { -AI_Velocity.x,velocity.y }; //Move left
				}
				//If AI is very close to the ball
				else if (ball_Distance <= jump_Distance && ball_Pos.y > (high_Ball))
				{
					//Jump to the left
					if (AI_Pos.x > (mid_Pos - 100)) velocity = { -AI_Velocity.x,velocity.y }; //Move left
					if (rectangle.pos.y == bottomBorder)
					{
						velocity = { velocity.x,-AI_Velocity.y }; //Jump
					}
				}
				//If AI is not at keeper_Pos or further
				else if (AI_Pos.x > keeper_Pos)
				{
					velocity = { -AI_Velocity.x,velocity.y }; //Move left
				}
				else velocity = { 0,velocity.y }; //Wait
			}

			//Ball moving backwards
			if (ball_Velocity.x > 0)
			{
				//If ball is far away
				if (ball_Distance >= far_Away)
				{
					//If AI is at goal_pos or further
					if (AI_Pos.x >= goal_Pos)
					{
						velocity = { 0,velocity.y }; //Wait
					}
					else velocity = { AI_Velocity.x,velocity.y }; //Move right
				}
				else
				{
					//If ball is pretty close to AI or closer
					if ((ball_Distance <= jump_Distance) && (ball_Pos.y > high_Ball))
					{
						//Jump to the left
						if (AI_Pos.x > (mid_Pos - 100)) velocity = { -AI_Velocity.x,velocity.y }; //Move left
						if (rectangle.pos.y == bottomBorder)
						{
							velocity = { velocity.x,-AI_Velocity.y }; //Jump
						}
					}
					else
					{
						//If AI is at keeper_Pos or further
						if (AI_Pos.x <= keeper_Pos)
						{
							velocity = { 0,velocity.y }; //Wait 
						}
						else velocity = { -AI_Velocity.x,velocity.y }; //Move left
					}
				}
			}

			//If ball X = 0
			if (ball_Velocity.x == 0)
			{
				//If AI is at jump_Distance or further
				if (ball_Distance <= jump_Distance)
				{
					//Jump to the left
					velocity = { -AI_Velocity.x,velocity.y };
					if (rectangle.pos.y == bottomBorder)
					{
						velocity = { velocity.x,-AI_Velocity.y };
					}
				}
				else if (AI_Pos.x > mid_Pos)
				{
					velocity = { -AI_Velocity.x,velocity.y }; //Move left
				}
			}
		}

		//If behind AI:
		if (ball_Pos.x > AI_Pos.x)
		{
			//If AI is at goal_pos or further
			if (AI_Pos.x >= goal_Pos)
			{
				velocity = { 0,velocity.y }; //Wait
			}
			else velocity = { AI_Velocity.x,velocity.y }; //Move right
		}
	}


	//LOW BALL
	else if (ball_Pos.y >= low_Ball)
	{
		//Ball in front of AI
		if (ball_Pos.x < AI_Pos.x)
		{
			//Ball moving forward
			if (ball_Velocity.x < 0)
			{
				//If AI is at keeper_Pos or further
				if (AI_Pos.x <= keeper_Pos)
				{
					velocity = { 0,velocity.y }; //Wait 
				}
				else velocity = { -AI_Velocity.x,velocity.y }; //Move left
			}

			//Ball moving backwards
			if (ball_Velocity.x > 0)
			{
				//If ball is far away
				if (ball_Distance >= far_Away)
				{
					velocity = { AI_Velocity.x,velocity.y }; //Move right
				}
				//If the ball is very close && AI is NOT at keeper_Pos or further
				else if ((ball_Distance <= jump_Distance) && (AI_Pos.x > keeper_Pos))
				{
					velocity = { -AI_Velocity.x,velocity.y }; //Move left
				}
				else velocity = { 0,velocity.y }; //Wait
			}

			//If ball X = 0
			if (ball_Velocity.x == 0)
			{
				//If AI is at mid_Pos or further
				if (AI_Pos.x <= mid_Pos)
				{
					velocity = { 0,velocity.y }; //Wait
				}
				else velocity = { -AI_Velocity.x,velocity.y }; //Move left
			}
		}

		//If behind AI:
		if (ball_Pos.x > AI_Pos.x)
		{
			//Ball moving forward
			if (ball_Velocity.x < 0)
			{
				//If ball is far away
				if (ball_Distance >= far_Away)
				{
					velocity = { AI_Velocity.x,velocity.y }; //Move right
				}
				//If ball is pretty close to AI or closer
				else if (ball_Distance <= (jump_Distance + 50))
				{
					//If Ball is at head height
					if (ball_Pos.y <= 425)
					{
						if (ball_Velocity.y < 0)
						{
							velocity = { 0,velocity.y }; //Wait
						}
						else if (AI_Pos.x > mid_Pos)
						{
							velocity = { -AI_Velocity.x,velocity.y }; //Move left
						}
					}
					else if (AI_Pos.x > mid_Pos)
					{
						velocity = { -AI_Velocity.x,velocity.y }; //Move left
					}
				}
				else velocity = { AI_Velocity.x,velocity.y }; //Move right
			}

			//Ball moving backwards
			if (ball_Velocity.x > 0)
			{
				//If the ball is very close
				if (ball_Distance <= jump_Distance)
				{
					velocity = { 0,velocity.y }; //Wait
				}
				else velocity = { AI_Velocity.x,velocity.y }; //Move right
			}

			//If ball X = 0
			if (ball_Velocity.x == 0)
			{
				//If Ball is higher than the AI
				if (ball_Pos.y < (bottomBorder - 85))
				{
					velocity = { AI_Velocity.x,velocity.y }; //Move right
				}
				else velocity = { 0,velocity.y }; //Wait
			}
		}
	}
}
