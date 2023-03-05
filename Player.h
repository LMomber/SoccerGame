#pragma once

#include "Character.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Player : public Character
{
public:
	Player();

	void Move(Ball& ball, int left, int right, int jump);
};

