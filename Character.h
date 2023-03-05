#pragma once

#include "Moving_Entity.h"
#include "Rectangle.h"
#include "Ball.h"

class Character : public Moving_Entity
{
public:
	void Update(float deltaTime);

	void Draw(Tmpl8::Surface* screen);

	const Tmpl8::vec2 GetPosition() const;
	void SetPosition(Tmpl8::vec2 pos);

	const int GetScore() const;
	void SetScore(int score);

	bool Collision(Ball& ball);

	void WallCollision();

	void BallCollision(Ball& ball);

	void Move(Ball& ball);

protected:
	int ballPosition{ 0 };
	int score{ 0 };
	Shape::Rectangle rectangle{ 80,60 };

private:
	Circle circle;
};

