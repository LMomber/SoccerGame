#pragma once

#include "template.h"
#include "surface.h"
#include "Circle.h"

/*********************************************************

	     Superclass for all moving entities.
	Moving entities include the ball, players and AI.

**********************************************************/

class MovingEntity
{
public:
	void SetVelocity(const Tmpl8::vec2 velocity);
	void SetGravity(const Tmpl8::vec2 gravity);

	void SetFrameFloat(const float frameFloat);

protected:
	Tmpl8::vec2 velocity{ 0 };
	Tmpl8::vec2 gravity{ 0 };

	//The bottom border is equal to the screenheight, minus the "grass" height of the background
	const int bottomBorder = ScreenHeight - 34;


	//Since sprite.SetFrame() takes an integer value, but a floating point is needed to in/decrement the value gradually:
	
	//Frame to set sprite to. Truncated version of frameFloat
	int frame{ 0 };
	//Gets incremented & decremented by deltaTime
	float frameFloat{ 0 };
	//
};

