#include "Goal.h"

Goal::Goal(int player)
	: rectangle{ 100,200 }
	, player_Goal{ player }
{}

void Goal::SetPosition(Tmpl8::vec2 pos)
{
	rectangle.pos = pos;
}

void Goal::BallCollision(Ball& ball)
{
	const float goal_Roof = rectangle.pos.y - rectangle.height;
	const float outerGoalPost_Left = rectangle.pos.x + (rectangle.width / 2.0f);
	const float outerGoalPost_Right = rectangle.pos.x - (rectangle.width / 2.0f);

	//For the left goal:
	if (player_Goal == 1)
	{
		//Ball center is inside the X-parameter of the goal
		if (ball.GetPosition().x <= outerGoalPost_Left + 10)
		{
			//Ball is above goal
			if (ball.GetPosition().y < goal_Roof)
			{
				//Ball touches the goal roof
				if (ball.GetPosition().y + ball.GetRadius() >= goal_Roof)
				{
					ball.SetPosition({ ball.GetPosition().x, goal_Roof - ball.GetRadius() });
					ball.SetVelocity({ ball.GetVelocity().x + 100, -ball.GetVelocity().y });
				}
			}
			//Ball is inside goal
			else if (ball.GetPosition().y > goal_Roof)
			{
				//Ball touches the goal roof
				if (ball.GetPosition().y - ball.GetRadius() <= goal_Roof)
				{
					ball.SetPosition({ ball.GetPosition().x, goal_Roof + ball.GetRadius() });
					ball.SetVelocity({ ball.GetVelocity().x, -ball.GetVelocity().y });
				}
			}
		}
	}

	//For the right goal:
	if (player_Goal == 2)
	{
		//Ball center is inside the X-parameter of the goal
		if (ball.GetPosition().x >= outerGoalPost_Right - 10)
		{
			//Ball is above goal
			if (ball.GetPosition().y < goal_Roof)
			{
				//Ball touches the goal roof
				if (ball.GetPosition().y + ball.GetRadius() >= goal_Roof)
				{
					ball.SetPosition({ ball.GetPosition().x, goal_Roof - ball.GetRadius() });
					ball.SetVelocity({ ball.GetVelocity().x - 100, -ball.GetVelocity().y });
				}
			}
			//Ball is inside goal
			else if (ball.GetPosition().y > goal_Roof)
			{
				//Ball touches the goal roof
				if (ball.GetPosition().y - ball.GetRadius() <= goal_Roof)
				{
					ball.SetPosition({ ball.GetPosition().x, goal_Roof + ball.GetRadius() });
					ball.SetVelocity({ ball.GetVelocity().x, -ball.GetVelocity().y });
				}
			}
		}
	}
}

bool Goal::IsGoal(const Ball& ball)
{
	const float goal_Roof = rectangle.pos.y - rectangle.height;
	const float outerGoalPost_Left = rectangle.pos.x + (rectangle.width / 2.0f);
	const float outerGoalPost_Right = rectangle.pos.x - (rectangle.width / 2.0f);

	/***************************************
	
		When inside the goal, return true
	
	****************************************/

	//For the goal on the left side
	if (player_Goal == 1)
	{
		if ((ball.GetPosition().x <= outerGoalPost_Left) && (ball.GetPosition().y > goal_Roof))
		{
			//Rotate between score-sounds
			switch (p2ScoreSoundRotation) {
			case 0:
				audioManager.PlayP2Score();
				p2ScoreSoundRotation++; 
				break;
			case 1:
				audioManager.PlayP2Score();
				p2ScoreSoundRotation = 0; 
				break;
			}
			return true;
		}
		else return false;
	}

	//For the goal on the right side
	if (player_Goal == 2)
	{
		if ((ball.GetPosition().x >= outerGoalPost_Right) && (ball.GetPosition().y > goal_Roof))
		{
			//Rotate between score-sounds
			if (p1ScoreSoundRotation == 0)
			{
				audioManager.PlayP1Score();
				p1ScoreSoundRotation++;
			}
			else
			{
				audioManager.PlayP1Score();
				p1ScoreSoundRotation = 0;
			}
			return true;
		}
		else return false;
	}

	return false;
}