#include "Character.h"

void Character::Update(float deltaTime)
{
	float maxVelocity_Y = 1500.0f;

	velocity += gravity;
	rectangle.bottomCenter += velocity * deltaTime;

	if (velocity.y <= -maxVelocity_Y)
	{
		velocity.y = -maxVelocity_Y;
	}
}

void Character::Draw(Tmpl8::Surface* screen)
{
	Tmpl8::vec2 botLeft{ (rectangle.bottomCenter.x - (rectangle.width / 2)), rectangle.bottomCenter.y };
	Tmpl8::vec2 topRight{ (rectangle.bottomCenter.x + (rectangle.width / 2)), (rectangle.bottomCenter.y - rectangle.height) };
	circle.center = { rectangle.bottomCenter.x, (rectangle.bottomCenter.y - rectangle.height) };
	int centerX = static_cast<int>(circle.center.x);
	int centerY = static_cast<int>(circle.center.y);
	circle.radius = static_cast<int>(rectangle.width / 2);

	screen->Line(botLeft.x, botLeft.y, botLeft.x, (botLeft.y - rectangle.height), color);
	screen->Line(botLeft.x, botLeft.y, topRight.x, botLeft.y, color);
	screen->Line(topRight.x, botLeft.y, topRight.x, (botLeft.y - rectangle.height), color);

	//Player head
	for (int i = (centerX - circle.radius); i < (centerX + circle.radius); i++)
	{
		int C = circle.radius;
		int A = (i - centerX);
		double B = sqrt((C * C) - (A * A)); //B^2 = C^2 - A^2
		int int_B = static_cast<int>(B);

		screen->Plot(centerX + A, centerY - int_B, color);
	}
	for (int i = (centerY - circle.radius); i < centerY; i++) //for y
	{
		int C = circle.radius;
		int B = (i - centerY);
		double A = sqrt((C * C) - (B * B)); //A^2 = C^2 - B^2
		int int_A = static_cast<int>(A);

		screen->Plot(centerX + int_A, centerY + B, color);
		screen->Plot(centerX - int_A, centerY + B, color);
	}
}

const Tmpl8::vec2 Character::GetPosition() const
{
	return rectangle.bottomCenter;
}

void Character::SetPosition(Tmpl8::vec2 pos)
{
	rectangle.bottomCenter = pos;
}

const int Character::GetScore() const
{
	return score;
}

void Character::SetScore(int score)
{
	this->score = score;
}

bool Character::Collision(Ball& ball)
{
	Tmpl8::vec2 collision = ball.GetPosition();

	float Character_Center = rectangle.bottomCenter.x;
	float Character_Leftside = rectangle.bottomCenter.x - (rectangle.width / 2.f);
	float Character_Rightside = rectangle.bottomCenter.x + (rectangle.width / 2.0f);
	float Character_Bottom = rectangle.bottomCenter.y;
	float Character_Top = rectangle.bottomCenter.y - rectangle.height;

	if (this->circle.Collides(ball.GetCircle()))
	{
		if (ball.GetPosition().x < Character_Center)
		{
			ballPosition = 1; //TOP LEFT
		}

		if (ball.GetPosition().x > Character_Center)
		{
			ballPosition = 2; //TOP RIGHT
		}
		return true;
	}
	else
	{
		//Ball is left of the player
		if (ball.GetPosition().x < Character_Leftside)
		{
			collision.x = Character_Leftside;

			if (ball.GetPosition().y > Character_Bottom)
			{
				collision.y = Character_Bottom;
				ballPosition = 5; //BOTTOM LEFT
			}
			else ballPosition = 3; //LEFT
		}
		//Ball is right of the player
		else if (ball.GetPosition().x > Character_Rightside)
		{
			collision.x = Character_Rightside;

			if (ball.GetPosition().y > Character_Bottom)
			{
				collision.y = Character_Bottom;
				ballPosition = 6; //BOTTOM RIGHT
			}
			else ballPosition = 4; //RIGHT
		}
		//Ball is under the player
		else if (ball.GetPosition().y > Character_Bottom)
		{
			collision.y = Character_Bottom;
			ballPosition = 7; //UNDER
		}

		//Ball is above the player
		if (ball.GetPosition().y < Character_Top)
		{
			collision.y = Character_Top;
		}
	}

	//std::cout << "ballPosition: " << ballPosition << "\n";

	float distanceX = ball.GetPosition().x - collision.x;
	float distanceY = ball.GetPosition().y - collision.y;

	double distance = sqrt(distanceX * distanceX + distanceY * distanceY);

	return (distance < ball.GetRadius()) ? true : false;
}

void Character::WallCollision()
{
	float leftBorder = rectangle.width / 2.0f + 1;
	float rightBorder = ScreenWidth - (rectangle.width / 2.0f) - 2;
	float bottomBorder = ScreenHeight - 2;
	float topBorder = static_cast<float>(rectangle.height);

	//Left border
	if (rectangle.bottomCenter.x <= leftBorder)
	{
		rectangle.bottomCenter = { leftBorder, rectangle.bottomCenter.y };
	}
	//Right border
	if (rectangle.bottomCenter.x >= rightBorder)
	{
		rectangle.bottomCenter = { rightBorder, rectangle.bottomCenter.y };
	}
	//Bottom border
	if (rectangle.bottomCenter.y >= bottomBorder)
	{
		rectangle.bottomCenter = { rectangle.bottomCenter.x, bottomBorder };
	}
	//Top border
	if (rectangle.bottomCenter.y <= topBorder)
	{
		rectangle.bottomCenter = { rectangle.bottomCenter.x, topBorder };
	}
}

void Character::BallCollision(Ball& ball)
{
	int minVelocity_Y = -50;

	int bumpBoost_X_Fast = 500;
	int bumpBoost_X_Normal = 300;
	int bumpBoost_X_Slow = 100;

	int bumpBoost_Y = 75;

	float leftCorner_Offset = rectangle.bottomCenter.x - (rectangle.width / 2.f) * (5.0f / 8.0f) - circle.radius;
	float leftMiddle_Offset = rectangle.bottomCenter.x - (rectangle.width / 2.f) * (2.0f / 3.0f);

	float rightCorner_Offset = rectangle.bottomCenter.x + (rectangle.width / 2.f) * (5.0f / 6.0f) + circle.radius;
	float rightMiddle_Offset = rectangle.bottomCenter.x + (rectangle.width / 2.f) * (2.0f / 3.0f);

	float top_Offset = rectangle.bottomCenter.x;

	if (Collision(ball))
	{
		switch (ballPosition)
		{
		case 1: //TOP LEFT
			ball.SetPosition({ (ball.GetPosition().x - 5), ball.GetPosition().y - 7 });
			if (ball.GetPosition().x <= leftCorner_Offset)
			{
				if (velocity.y < minVelocity_Y)
				{
					ball.SetVelocity({ ball.GetVelocity().x - bumpBoost_X_Fast, -ball.GetVelocity().y + velocity.y - bumpBoost_Y });
				}
				else
				{
					ball.SetVelocity({ ball.GetVelocity().x - bumpBoost_X_Fast, -ball.GetVelocity().y + minVelocity_Y });
				}
			}
			else if (ball.GetPosition().x <= leftMiddle_Offset)
			{
				if (velocity.y < minVelocity_Y)
				{
					ball.SetVelocity({ ball.GetVelocity().x - bumpBoost_X_Normal, -ball.GetVelocity().y + velocity.y - bumpBoost_Y });
				}
				else
				{
					ball.SetVelocity({ ball.GetVelocity().x - bumpBoost_X_Normal, -ball.GetVelocity().y + minVelocity_Y });
				}
			}
			else if (ball.GetPosition().x <= top_Offset)
			{
				if (velocity.y < minVelocity_Y)
				{
					ball.SetVelocity({ ball.GetVelocity().x - bumpBoost_X_Slow, -ball.GetVelocity().y + velocity.y - bumpBoost_Y });
				}
				else
				{
					ball.SetVelocity({ ball.GetVelocity().x - bumpBoost_X_Slow, -ball.GetVelocity().y + minVelocity_Y });
				}
			}
			break;

		case 2: //TOP RIGHT
			ball.SetPosition({ (ball.GetPosition().x + 5), ball.GetPosition().y - 7 });
			if (ball.GetPosition().x >= rightCorner_Offset)
			{
				if (velocity.y < minVelocity_Y)
				{
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Fast, -ball.GetVelocity().y + velocity.y + bumpBoost_Y });
				}
				else
				{
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Fast, -ball.GetVelocity().y + minVelocity_Y });
				}
			}
			else if (ball.GetPosition().x >= rightMiddle_Offset)
			{
				if (velocity.y < minVelocity_Y)
				{
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Normal, -ball.GetVelocity().y + velocity.y + bumpBoost_Y });
				}
				else
				{
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Normal, -ball.GetVelocity().y + minVelocity_Y });
				}
			}
			else if (ball.GetPosition().x >= top_Offset)
			{
				if (velocity.y < minVelocity_Y)
				{
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Slow, -ball.GetVelocity().y + velocity.y + bumpBoost_Y });
				}
				else
				{
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Slow, -ball.GetVelocity().y + minVelocity_Y });
				}
			}
			break;

		case 3: //LEFT
			if (velocity.x != 0)
			{
				ball.SetPosition({ (ball.GetPosition().x - 5), ball.GetPosition().y });
				ball.SetVelocity({ velocity.x + 10, ball.GetVelocity().y });
			}
			else if (velocity.x == 0)
			{
				ball.SetVelocity({ -ball.GetVelocity().x, ball.GetVelocity().y });
			}
			break;

		case 4: //RIGHT
			if (velocity.x != 0)
			{
				ball.SetPosition({ (ball.GetPosition().x + 5), ball.GetPosition().y });
				ball.SetVelocity({ velocity.x + 10, ball.GetVelocity().y });
			}
			else if (velocity.x == 0)
			{
				ball.SetVelocity({ -ball.GetVelocity().x, ball.GetVelocity().y });
			}
			break;

		case 5: //BOTTOM LEFT
			ball.SetPosition({ ball.GetPosition().x - 10, ball.GetPosition().y + 20 });
			ball.SetVelocity({ ball.GetVelocity().x - 200, -ball.GetVelocity().y + 300 });
			break;

		case 6: //BOTTOM RIGHT
			ball.SetPosition({ ball.GetPosition().x + 10, ball.GetPosition().y + 20 });
			ball.SetVelocity({ ball.GetVelocity().x + 200, -ball.GetVelocity().y + 300 });
			break;

		case 7: //UNDER
			ball.SetPosition({ ball.GetPosition().x, ball.GetPosition().y + 20 });
			ball.SetVelocity({ ball.GetVelocity().x + velocity.x, -ball.GetVelocity().y + 300 });
			break;
		}
	}
}

void Character::Move(Ball& ball)
{
	int minVelocity_Y = -50;

	int bumpBoost_X_Fast = 500;
	int bumpBoost_X_Normal = 300;
	int bumpBoost_X_Slow = 100;

	int bumpBoost_Y = 50;

	float leftCorner_Offset = rectangle.bottomCenter.x - (rectangle.width / 2.f) * (5.0f / 8.0f) - circle.radius;
	float leftMiddle_Offset = rectangle.bottomCenter.x - (rectangle.width / 2.f) * (2.0f / 3.0f);

	float rightCorner_Offset = rectangle.bottomCenter.x + (rectangle.width / 2.f) * (5.0f / 6.0f) + circle.radius;
	float rightMiddle_Offset = rectangle.bottomCenter.x + (rectangle.width / 2.f) * (2.0f / 3.0f);

	float top_Offset = rectangle.bottomCenter.x;

	WallCollision();

	BallCollision(ball);
}