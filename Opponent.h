#pragma once

#include "Character.h"

class Opponent : public Character
{
public:
	Opponent(Ball& ball);

	void Update(float deltaTime);

	void Move();

	const bool IsHigh() const;

	const bool IsJumpHeight() const;

	const bool IsLow() const;

	const bool InFront() const;

private:
	Ball& ball;
	int high_Ball{ 200 };
	int low_Ball{ 350 };
};

