#pragma once

#include "rectangle.h"
#include "surface.h"

#include "AudioManager.h"

/**********************************************************************

	This class holds:

	- The button functionalities (Draw, OnClick, MouseOnButton)
	- The different buttontypes, gamestates and opponents.

***********************************************************************/

class Button
{
public:
	//Button type
	enum Type
	{
		StartButton,
		PlayAgain,
		ChangeOpponent,
		BackToMenu,
		TimeFormat,
		GoToSettings,
		Information,
		ChangeControlls
	};

	//Gamestate
	enum State {
		Menu,
		Settings,
		InfoScreen,
		Playing,
		Start,
		Countdown,
		Reset
	};

	//Opponent type
	enum Opponent {
		PvP, //Player vs Player
		PvHAI, //Player vs hard AI
		PvEAI //Player vs easy AI
	};

	explicit Button(const float a_X, const float a_Y, const int width, const int height, Tmpl8::Pixel borderColor, const char* label, 
		int& mouseX, int& mouseY, int& mouseReleased, int& mousePressed, const int textWidth, const Tmpl8::Pixel textColor, const Type buttonType);

	void Draw(Tmpl8::Surface* screen, const bool border);

	void OnClick(int& mouseReleased, State& gameState);
	void OnClick(int& mouseReleased, Opponent& enemyOption);
	void OnClick(int& mouseReleased, int& action);

	bool MouseOnButton();

private:
	Shape::Rectangle rectangle;
	const Type buttonType;
	const Tmpl8::Pixel borderColor;
	const Tmpl8::Pixel textColor;
	const int textWidth;
	const char* label;

	//Mouse input
	int& mouseX;
	int& mouseY;
	int& mouseReleased;
	int& mousePressed;

	//(x1, y1) is the bottomleft corner
	const int x1 = static_cast<int>(rectangle.pos.x);
	const int y1 = static_cast<int>(rectangle.pos.y);

	//(x2, y2) is the topright corner
	const int x2 = static_cast<int>(rectangle.pos.x + rectangle.width);
	const int y2 = static_cast<int>(rectangle.pos.y - rectangle.height);

	AudioManager audioManager = AudioManager::getInstance();
};

