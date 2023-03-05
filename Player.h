#pragma once

#include "Character.h"
#include "GamePad.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Player : public Character
{
public:
	Player();

	void Move(Ball& ball, int left, int right, int jump);

	void Controller(Ball& ball, Graphics::GamePadState& state);
};

