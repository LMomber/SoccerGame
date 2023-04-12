#pragma once

#include "Character.h"

/*************************************************************************************************************

	                       This class holds the "brain" of the AI. 

	The AI's actions are based on the location & velocity of the ball, and the location of the AI itself:

	- Is the ball located high, on jump height, or low?
	- Is the ball in front or behind the AI?
	- In what direction does the ball move?
	- How fast does the ball move? 
	- What is the distance between the ball and the AI?
	- What is the X-position of the AI?

	for a more detailed explanation, see appendix "Flowchart_AI.drawio".
	Draw.io files can be opened on https://www.diagrams.net

	Inherited: MovingEntity, Character

**************************************************************************************************************/

class AI : public Character
{
public:
	explicit AI(const int mid_Pos, const int keeper_Pos, const int goal_Pos);

	void Move(const Ball& ball);

private:
	//AI X-positions
	int mid_Pos;
	int keeper_Pos;
	int goal_Pos;

	//AI velocity
	const Tmpl8::vec2 AI_Velocity = { 500.0f, 3000.0f };

	//Constant distances between AI and ball
	const int jump_Distance = (rectangle.width / 2) + 100;
	const int far_Away = 600;

	//Ball height
	const int high_Ball = 200;
	const int low_Ball = 350;

	//Ball velocity
	const int fast_Ball = 1000;
	const int mid_Ball = 400;
	const int slow_Ball = 300;
};

