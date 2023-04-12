#pragma once

#include "MovingEntity.h"

/*******************************************************************************
	
	This is the ball class. Mind that the only collision in 
	this class is the ball-wall collision. The ball-goal collision can be 
	found in "Goal.h" and the ball-player/AI collision inside "Character.h".

	Inherited: MovingEntity
 
********************************************************************************/

class Ball : public MovingEntity
{
public:
	Ball();

	void Update(float deltaTime);

	//Resets gravity, velocity, position & frame
	void Reset(Tmpl8::Sprite& sprite);

	//Sets the sprite frame, depending on the velocity of the ball
	void Frames(float deltaTime, Tmpl8::Sprite& sprite);

	const Tmpl8::vec2& GetGravity() const;
	const Tmpl8::vec2& GetVelocity() const;
	const Tmpl8::vec2& GetPosition() const;
	void SetPosition(Tmpl8::vec2 pos);

	const int GetRadius() const;
	void SetRadius(int radius);

	const Circle& GetCircle() const;

	//Bool to see if the ball touches the bottom border
	bool IsGround() const;

	void WallCollision();

private:
	Circle circle;

	const Tmpl8::vec2 maxVelocity{ 1200.0f, -1000.0f };
};

