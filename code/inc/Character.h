#pragma once

#include "MovingEntity.h"
#include "Rectangle.h"
#include "Ball.h"

#include "AudioManager.h"

/*******************************************************************************

	The character class holds the shared code of the player & AI classes.
	This includes every function, aside from the move-function.

	Inherited: MovingEntity

********************************************************************************/

class Character : public MovingEntity
{
public:
	//Ball position relative to the character
	enum class BallPosition
	{
		TopLeft,
		TopRight,
		Left,
		Right,
		BottomLeft,
		BottomRight,
		Underneath
	};

	void Update(const float deltaTime);

	//Resets gravity, velocity, position & frame
	void Reset(const float a_X, Tmpl8::Sprite& sprite);

	//Sets the sprite frame, depending on the velocity and position of the character
	void Frames(const float deltaTime, Tmpl8::Sprite& sprite);

	const Tmpl8::vec2& GetPosition() const;
	void SetPosition(const Tmpl8::vec2 pos);

	const int GetScore() const;
	void SetScore(const int score);

	const int GetHit() const;
	void setHit(int hit);

	//Character-ball collision check
	bool CollisionCheck(Ball& ball);

	void WallCollision();
	void BallCollision(Ball& ball);
	void GoalCollision();

protected:
	Shape::Rectangle rectangle{ 80,60 };
	BallPosition ballPosition{ BallPosition::TopLeft };

	//The spherical collision of the character
	Circle head;
private:

	const float maxVelocity_Y = 1000.0f;
	int score{ 0 };

	int hit{ 0 };
};

