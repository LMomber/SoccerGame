#pragma once

#include "Rectangle.h"
#include "Ball.h"

class Goal
{
public:
	Goal();

	Goal(int player);

	void Update(Ball& ball);

	const Tmpl8::vec2& GetPosition() const;
	void SetPosition(Tmpl8::vec2 pos);

	void Draw(Tmpl8::Surface* screen);

	const Tmpl8::Pixel GetColor() const;
	void SetColor(Tmpl8::Pixel color);

	void Collision(Ball& ball);

	bool Scored(Ball& ball);

private:
	Shape::Rectangle rectangle;
	Tmpl8::Pixel color{ Tmpl8::GreenMask };
	int player_Goal;
};