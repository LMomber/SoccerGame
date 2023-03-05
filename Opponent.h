#pragma once

#include "Character.h"

class Opponent : public Character
{
public:
	Opponent();

	void Move(Ball& ball);
};

