#pragma once

#include "GamePad.hpp"
#include "Ball.h"

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
	int player;
};

