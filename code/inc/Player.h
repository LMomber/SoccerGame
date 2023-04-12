#pragma once

#include "Character.h"
#include "Controller.h"

#include "AudioPlayer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/*****************************************************************************************

	This class holds the code for controlling the player. 
	The players can be controlled using keyboard, and/or XBOX-controllers.

	The keyboards controls are:

	Player 1: a (left), d (right), w & space (up).

	Player 2: arrow keys.

	When playing with only one player (PvAI), player 1 can also use the arrow keys.

	Inherited: MovingEntity, Character

******************************************************************************************/

class Player : public Character
{
public:
	//Player 1
	Player(const int a, const int d, const int w, const int space, const  Controller& controller);

	//Player 2
	Player(const int leftKey, const int rightKey, const int upKey, const  Controller& controller);

	void Move();

private:
	const Controller& controller;
	const int leftKey;
	const int rightKey;
	const int upKey1;
	const int upKey2{ 0 };
};

