#pragma once

#include "Moving_Entity.h"

class Ball : public Moving_Entity
{
public:
	Ball();

	void Update(float deltaTime);

	void Draw(Tmpl8::Surface* screen);

	const Tmpl8::vec2& GetPosition() const;
	void SetPosition(Tmpl8::vec2 pos);

	const int GetRadius() const;
	void SetRadius(int radius);

	const Circle& GetCircle() const;

	void WallCollision();

private:
	Circle circleCenter;
};

