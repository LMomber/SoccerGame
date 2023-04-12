#pragma once

#include "GamePad.hpp"
#include "Ball.h"

/**************************************************************************************

	This class is used to connect XBOX-controllers and read/write data from/to them. 

***************************************************************************************/

class Controller
{
public:
	explicit Controller(int player = 0);

	void Update(float deltaTime);

	const float GetThumbStickL() const;

	const bool GetButtonA() const;

	void SetVibration(signed int vibration);

	bool IsConnected();

private:
	Graphics::GamePadState state;
	unsigned int vibration{ 0 };

	//Player = which player the controller belongs to: P1 or P2.
	int player;
};

