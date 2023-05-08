#include "Character.h"

void Character::Update(const float deltaTime)
{
	//Physics
	velocity += gravity * (100 * deltaTime);
	rectangle.pos += velocity * deltaTime;

	//Cap the Y-velocity of the characters
	if (velocity.y <= -maxVelocity_Y) velocity.y = -maxVelocity_Y;
}

//Resets gravity, velocity, position & frame
void Character::Reset(const float a_X, Tmpl8::Sprite& sprite)
{
	gravity = { 0, 40 };
	velocity = { 0 };
	rectangle.pos = { a_X, static_cast<float>(bottomBorder) };
	sprite.SetFrame(0);
}

//Sets the sprite frame, depending on the velocity and position of the character
void Character::Frames(const float deltaTime, Tmpl8::Sprite& sprite)
{
	frame = static_cast<int>(frameFloat);

	//Value in in/decrement frameFloat by
	float walkingSpeed = 15 * deltaTime;

	/*********************************
				frames:

		0:		Idle frame
		1 - 4:	Walking frames
		5:		Jump frame

	**********************************/

	//If the player jumps:
	if (rectangle.pos.y < bottomBorder) frameFloat = 5;
	//else
	else
	{
		//Walking to the right
		if (velocity.x > 0)
		{
			if (frameFloat < 4)
			{
				frameFloat += walkingSpeed;
			}
			else
			{
				frameFloat = 1;
			}
		}

		//Walking to the left
		if (velocity.x < 0)
		{
			if (frameFloat > 1)
			{
				frameFloat -= walkingSpeed;
			}
			else
			{
				frameFloat = 4;
			}
		}

		//Standing still
		if (velocity.x == 0) frameFloat = 0;
	}

	sprite.SetFrame(frame);
}

const Tmpl8::vec2& Character::GetPosition() const
{
	return rectangle.pos;
}

void Character::SetPosition(const Tmpl8::vec2 pos)
{
	rectangle.pos = pos;
}

const int Character::GetScore() const
{
	return score;
}

void Character::SetScore(const int score)
{
	this->score = score;
}

const int Character::GetHit() const
{
	return hit;
}

void Character::setHit(int hit)
{
	this->hit = hit;
}

///Code derived from youtube video:
///https://www.youtube.com/watch?v=r0wAEi86vTA&list=PLpgJ68tsaWmh8ffivXgoOxhDaPh8p8-3w&index=12
//Character-ball collision check
bool Character::CollisionCheck(Ball& ball)
{
	Tmpl8::vec2 collidingPoint = ball.GetPosition();

	float CharacterCenter_X = rectangle.pos.x;
	float Character_Leftside = rectangle.pos.x - (rectangle.width / 2.f);
	float Character_Rightside = rectangle.pos.x + (rectangle.width / 2.0f);
	float Character_Bottom = rectangle.pos.y;
	float Character_Top = rectangle.pos.y - rectangle.height;

	//Character head
	head.center = { rectangle.pos.x, (rectangle.pos.y - rectangle.height) };
	head.radius = static_cast<int>(rectangle.width / 2);

	/******************************************************************************************************

		See if the character head collides with the ball:

		If so:

		- check and store the position of the ball relative to the character (ballPosition).

		- return true.

		If not:

		- check and store the position of the ball relative to the character (ballPosition).

		- determine the closest colliding point on the character,
		  and with that, the distance between the character and the ball.

		- If the distance between the centerpoint of the ball and the
		  colliding point is smaller than the radius of the ball, return true.


		Reference for the circle-rectangle collision:
		https://www.youtube.com/watch?v=r0wAEi86vTA&list=PLpgJ68tsaWmh8ffivXgoOxhDaPh8p8-3w&index=12
	*******************************************************************************************************/

	//Circle-circle collision
	if (this->head.Collides(ball.GetCircle()))
	{
		if (ball.GetPosition().x < CharacterCenter_X)
		{
			ballPosition = BallPosition::TopLeft; //TOPLEFT
		}

		if (ball.GetPosition().x > CharacterCenter_X)
		{
			ballPosition = BallPosition::TopRight; //TOPRIGHT
		}
		hit = 1;
		return true;
	}
	else
	{
		//Ball is left of the character
		if (ball.GetPosition().x < Character_Leftside)
		{
			collidingPoint.x = Character_Leftside;

			//Ball is also underneath the character
			if (ball.GetPosition().y > Character_Bottom)
			{
				collidingPoint.y = Character_Bottom;
				ballPosition = BallPosition::BottomLeft; //BOTTOMLEFT
			}
			else ballPosition = BallPosition::Left; //LEFT
		}
		//Ball is right of the character
		else if (ball.GetPosition().x > Character_Rightside)
		{
			collidingPoint.x = Character_Rightside;

			//Ball is also underneath the character
			if (ball.GetPosition().y > Character_Bottom)
			{
				collidingPoint.y = Character_Bottom;
				ballPosition = BallPosition::BottomRight; //BOTTOMRIGHT
			}
			else ballPosition = BallPosition::Right; //RIGHT
		}
		//Ball is under the character
		else if (ball.GetPosition().y > Character_Bottom)
		{
			collidingPoint.y = Character_Bottom;
			ballPosition = BallPosition::Underneath; //UNDER
		}

		//Ball is above the character
		if (ball.GetPosition().y < Character_Top)
		{
			collidingPoint.y = Character_Top;
		}
	}

	float distanceX = ball.GetPosition().x - collidingPoint.x;
	float distanceY = ball.GetPosition().y - collidingPoint.y;

	double distance = sqrt(distanceX * distanceX + distanceY * distanceY);

	if (distance < ball.GetRadius())
	{
		hit = 1;
		return true;
	}
	else
	{
		return false;
	}
}

void Character::WallCollision()
{
	const float leftBorder = rectangle.width / 2.0f;
	const float rightBorder = ScreenWidth - (rectangle.width / 2.0f);
	const float topBorder = static_cast<float>(rectangle.height);

	/*******************************************************************

			When hitting a border, set the corresponding position.

	********************************************************************/

	//Left border
	if (rectangle.pos.x <= leftBorder)
	{
		rectangle.pos = { leftBorder, rectangle.pos.y };
	}
	//Right border
	if (rectangle.pos.x >= rightBorder)
	{
		rectangle.pos = { rightBorder, rectangle.pos.y };
	}
	//Bottom border
	if (rectangle.pos.y >= bottomBorder)
	{
		rectangle.pos = { rectangle.pos.x, static_cast<float>(bottomBorder) };
	}
	//Top border
	if (rectangle.pos.y <= topBorder)
	{
		rectangle.pos = { rectangle.pos.x, topBorder };
	}
}

void Character::BallCollision(Ball& ball)
{
	const int minVelocity_Y = -50;

	const int factor = 1;
	const int bumpBoost_X_Fast = 1000 * factor;
	const int bumpBoost_X_Normal = 500 * factor;
	const int bumpBoost_X_Slow = 100 * factor;

	const int bumpBoost_Y = 0; //75

	float leftCorner_Offset = rectangle.pos.x - (rectangle.width / 2.f) * (5.0f / 8.0f) - head.radius;
	float leftMiddle_Offset = rectangle.pos.x - (rectangle.width / 2.f) * (2.0f / 3.0f);

	float rightCorner_Offset = rectangle.pos.x + (rectangle.width / 2.f) * (5.0f / 6.0f) + head.radius;
	float rightMiddle_Offset = rectangle.pos.x + (rectangle.width / 2.f) * (2.0f / 3.0f);

	float top_Offset = rectangle.pos.x;

	/*******************************************************************************************************************************

		When the collision check is true:

		Depending on the ball position, change the velocity & position of the ball.

		If the ball lands on the head of the character, the ball gets extra X-velocity depending on the location of the ball:

		- On the middle of the head:	a small boost
		- A bit more to the side:		a medium boost
		- On the edge of the head:		a big boost

		If the ball lands the character's head while he jumps up, the ball gets extra Y-velocity.

	********************************************************************************************************************************/

	if (CollisionCheck(ball))
	{
		switch (ballPosition)
		{
		case BallPosition::TopLeft: //TOP LEFT
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

		case BallPosition::TopRight: //TOP RIGHT
			ball.SetPosition({ (ball.GetPosition().x + 5), ball.GetPosition().y - 7 });
			if (ball.GetPosition().x >= rightCorner_Offset)
			{
				if (velocity.y < minVelocity_Y)
				{
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Fast, -ball.GetVelocity().y + velocity.y - bumpBoost_Y });
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
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Normal, -ball.GetVelocity().y + velocity.y - bumpBoost_Y });
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
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Slow, -ball.GetVelocity().y + velocity.y - bumpBoost_Y });
				}
				else
				{
					ball.SetVelocity({ ball.GetVelocity().x + bumpBoost_X_Slow, -ball.GetVelocity().y + minVelocity_Y });
				}
			}
			break;

		case BallPosition::Left: //LEFT
			if (velocity.x != 0)
			{
				ball.SetPosition({ (ball.GetPosition().x - 10), ball.GetPosition().y });
				ball.SetVelocity({ velocity.x - 200, ball.GetVelocity().y });
			}
			else if (velocity.x == 0)
			{
				ball.SetVelocity({ -ball.GetVelocity().x, ball.GetVelocity().y });
			}
			break;

		case BallPosition::Right: //RIGHT
			if (velocity.x != 0)
			{
				ball.SetPosition({ (ball.GetPosition().x + 10), ball.GetPosition().y });
				ball.SetVelocity({ velocity.x + 200, ball.GetVelocity().y });
			}
			else if (velocity.x == 0)
			{
				ball.SetVelocity({ -ball.GetVelocity().x, ball.GetVelocity().y });
			}
			break;

		case BallPosition::BottomLeft: //BOTTOM LEFT
			ball.SetPosition({ ball.GetPosition().x - 10, ball.GetPosition().y + 20 });
			ball.SetVelocity({ ball.GetVelocity().x - 200, -ball.GetVelocity().y + 300 });
			break;

		case BallPosition::BottomRight: //BOTTOM RIGHT
			ball.SetPosition({ ball.GetPosition().x + 10, ball.GetPosition().y + 20 });
			ball.SetVelocity({ ball.GetVelocity().x + 200, -ball.GetVelocity().y + 300 });
			break;

		case BallPosition::Underneath: //UNDER
			ball.SetPosition({ ball.GetPosition().x, ball.GetPosition().y + 20 });
			ball.SetVelocity({ ball.GetVelocity().x + velocity.x, -ball.GetVelocity().y + 300 });
			break;
		}
	}
}

void Character::GoalCollision()
{
	//Goal positions
	const float goal_Roof = ScreenHeight - 232;

	const float outerGoalPost_Left = 125;
	const float outerGoalPost_Right = ScreenWidth - 125;

	//Character positions
	const int characterHeight = rectangle.height + head.radius;

	float left = rectangle.pos.x - (rectangle.width / 2.0f);
	float right = rectangle.pos.x + (rectangle.width / 2.0f);

	head.radius = static_cast<int>(rectangle.width / 2);

	/************************************************************

		Checks the collision between a character and the goals

	*************************************************************/


	//If the character is inside one of the goals
	if ((rectangle.pos.x <= outerGoalPost_Left) || (rectangle.pos.x >= outerGoalPost_Right))
	{
		//If the highest point of the character touches the goal roof, make Y-velocity 0
		if ((rectangle.pos.y - characterHeight) <= goal_Roof)
		{
			rectangle.pos = { rectangle.pos.x, goal_Roof + characterHeight };
			velocity = { velocity.x, 0 };
		}
	}
	//Else if the character jumps against the side of one of the goals, make X-velocity 0
	else if ((rectangle.pos.y - characterHeight) <= goal_Roof && rectangle.pos.x <= outerGoalPost_Left + 10) //LEFT GOAL
	{
		rectangle.pos = { outerGoalPost_Left + 10, rectangle.pos.y };
		velocity = { 0, velocity.y };
	}
	else if ((rectangle.pos.y - characterHeight) <= goal_Roof && rectangle.pos.x >= outerGoalPost_Right - 10) //RIGHT GOAL
	{
		rectangle.pos = { outerGoalPost_Right - 10, rectangle.pos.y };
		velocity = { 0, velocity.y };
	}
}