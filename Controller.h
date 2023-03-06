#pragma once

#include "GamePad.hpp"
#include "Ball.h"

class Controller
{
public:
	Controller();

	Controller(int player);

	void Update(float deltaTime);

	const float GetThumbStickL() const;

	const bool GetButtonA() const;

	void SetVibration(signed int vibration);

private:
	Graphics::GamePadState state;
	unsigned int vibration{ 0 };
	int player;
};

