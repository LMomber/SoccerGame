#pragma once

#include "Rectangle.h"
#include "Ball.h"

#include "AudioManager.h"

/******************************************************************

	This class sets the width, height and position of the goals,
	goal-ball collision and checks if there has been scored.

 ******************************************************************/

class Goal
{
public:
	Goal(const int player);

	void SetPosition(const Tmpl8::vec2 pos);

	bool IsGoal(const Ball& ball);

	void BallCollision(Ball& ball);

private:
	Shape::Rectangle rectangle;
	int player_Goal;

	int p1ScoreSoundRotation{ 0 };
	int p2ScoreSoundRotation{ 0 };

	AudioManager audioManager = AudioManager::getInstance();
};