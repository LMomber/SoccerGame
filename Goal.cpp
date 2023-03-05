#include "Goal.h"

#include <iostream>

Goal::Goal()
	:rectangle{ 100,200 }
{}

Goal::Goal(int player)
	:rectangle{ 100,200 }
	, player_Goal{ player }
{}

void Goal::Update(Ball& ball)
{
	Collision(ball);
}

const Tmpl8::vec2& Goal::GetPosition() const
{
	return rectangle.bottomCenter;
}

void Goal::SetPosition(Tmpl8::vec2 pos)
{
	rectangle.bottomCenter = pos;
}

void Goal::Draw(Tmpl8::Surface* screen)
{
	Tmpl8::vec2 bottomLeft{ rectangle.bottomCenter.x - (rectangle.width / 2) , rectangle.bottomCenter.y };
	Tmpl8::vec2 topRight{ rectangle.bottomCenter.x + (rectangle.width / 2) , rectangle.bottomCenter.y - rectangle.height };

	if (player_Goal == 1)
	{
		screen->Line(bottomLeft.x, bottomLeft.y, bottomLeft.x, topRight.y, color);
		screen->Line(bottomLeft.x, topRight.y, topRight.x, topRight.y, color);
	}

	if (player_Goal == 2)
	{
		screen->Line(topRight.x, bottomLeft.y, topRight.x, topRight.y, color);
		screen->Line(topRight.x, topRight.y, bottomLeft.x, topRight.y, color);
	}
}

const Tmpl8::Pixel Goal::GetColor() const
{
	return color;
}

void Goal::SetColor(Tmpl8::Pixel color)
{
	this->color = color;
}

void Goal::Collision(Ball& ball)
{
	float goal_Roof = rectangle.bottomCenter.y - rectangle.height;
	float left = rectangle.bottomCenter.x - (rectangle.width / 2.0f);
	float right = rectangle.bottomCenter.x + (rectangle.width / 2.0f);

	//Ball is above the goal roof
	if (player_Goal == 1)
	{
		if (ball.GetPosition().x <= rectangle.bottomCenter.x + (rectangle.width / 2.0f))
		{
			if (ball.GetPosition().y < goal_Roof)
			{
				if (ball.GetPosition().y + ball.GetRadius() >= goal_Roof)
				{
					ball.SetPosition({ ball.GetPosition().x, goal_Roof - ball.GetRadius() });
					ball.SetVelocity({ ball.GetVelocity().x, -ball.GetVelocity().y });
				}
			}
			else if (ball.GetPosition().y > goal_Roof)
			{
				if (ball.GetPosition().y - ball.GetRadius() <= goal_Roof)
				{
					ball.SetPosition({ ball.GetPosition().x, goal_Roof + ball.GetRadius() });
					ball.SetVelocity({ ball.GetVelocity().x, -ball.GetVelocity().y });
				}
			}
		}
	}

	if (player_Goal == 2)
	{
		if (ball.GetPosition().x >= rectangle.bottomCenter.x - (rectangle.width / 2.0f))
		{
			if (ball.GetPosition().y < goal_Roof)
			{
				if (ball.GetPosition().y + ball.GetRadius() >= goal_Roof)
				{
					ball.SetPosition({ ball.GetPosition().x, goal_Roof - ball.GetRadius() });
					ball.SetVelocity({ ball.GetVelocity().x, -ball.GetVelocity().y });
				}
			}
			else if (ball.GetPosition().y > goal_Roof)
			{
				if (ball.GetPosition().y - ball.GetRadius() <= goal_Roof)
				{
					ball.SetPosition({ ball.GetPosition().x, goal_Roof + ball.GetRadius() });
					ball.SetVelocity({ ball.GetVelocity().x, -ball.GetVelocity().y });
				}
			}
		}
	}
}

bool Goal::Scored(Ball& ball)
{
	float goal_Roof = rectangle.bottomCenter.y - rectangle.height;
	float left = rectangle.bottomCenter.x - (rectangle.width / 2.0f);
	float right = rectangle.bottomCenter.x + (rectangle.width / 2.0f);

	if (player_Goal == 1)
	{
		if ((ball.GetPosition().x <= right) && (ball.GetPosition().y > goal_Roof))
		{
			return true;
		}
		else return false;
	}

	if (player_Goal == 2)
	{
		if ((ball.GetPosition().x >= left) && (ball.GetPosition().y > goal_Roof))
		{
			return true;
		}
		else return false;
	}
}
