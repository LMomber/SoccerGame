#include "Button.h"
#include <cstring>

Button::Button(const float a_X, const float a_Y, const int width, const int height, Tmpl8::Pixel borderColor, const char* label,
	int& mouseX, int& mouseY, int& mouseReleased, int& mousePressed, int textWidth, Tmpl8::Pixel textColor, Type buttonType)
	: rectangle{ width, height, {a_X, a_Y} }
	, borderColor{ borderColor }
	, label{ label }
	, mouseX{ mouseX }
	, mouseY{ mouseY }
	, mouseReleased{ mouseReleased }
	, mousePressed{ mousePressed }
	, textWidth{ textWidth }
	, textColor{ textColor }
	, buttonType{ buttonType }
{}

void Button::Draw(Tmpl8::Surface* screen, const bool border)
{
	const Tmpl8::vec2 middle{ static_cast<float>(x1 + ((x2 - x1) / 2)), static_cast<float>(y1 + ((y2 - y1) / 2)) };

	//If the border-bool is true, draw a border around the button
	if (border)
	{
		screen->Line(x1, y1, x1, y2, borderColor);
		screen->Line(x1, y1, x2, y1, borderColor);
		screen->Line(x1, y2, x2, y2, borderColor);
		screen->Line(x2, y1, x2, y2, borderColor);
	}

	//If the mouse is on the button, as an indicator, draw an extra border around the button
	if (MouseOnButton())
	{
		screen->Line(x1 - 1, y1, x1 - 1, y2, borderColor);
		screen->Line(x1, y1 - 1, x2, y1 - 1, borderColor);
		screen->Line(x1, y2 + 1, x2, y2 + 1, borderColor);
		screen->Line(x2 + 1, y1, x2 + 1, y2, borderColor);

		//If the mouse is pressed, as an indicator, make the whole button white
		if (mousePressed)
		{
			for (int y = y2 + 1; y < y1; y++)
			{
				screen->Line(x1, y, x2, y, 0xdddddd);
			}
		}
	}

	//Print the button label in the middle of the button
	screen->Print(label, middle.x - (textWidth * strlen(label) * 3), middle.y - textWidth * 2, textColor, textWidth);
}

//Start, reset, settings & back-buttons. The start-case is used for the start & reset buttons.
void Button::OnClick(int& mouseReleased, State& gameState)
{
	if (MouseOnButton())
	{
		if (mouseReleased)
		{
			switch (buttonType) {
			case StartButton:
				gameState = Start;
				break;
			case GoToSettings:
				gameState = Settings;
				break;
			case BackToMenu:
				gameState = Menu;
				break;
			case Information:
				gameState = InfoScreen;
				break;
			}
			audioManager.PlayClick();
			this->mouseReleased = mouseReleased = 0;
		}
	}
}

//Change-opponent button
void Button::OnClick(int& mouseReleased, Opponent& enemyOption)
{
	if (MouseOnButton())
	{
		if (mouseReleased)
		{
			switch (enemyOption) {
			case PvEAI:
				enemyOption = PvHAI;
				label = "hard AI";
				break;
			case PvHAI:
				enemyOption = PvP;
				label = "player";
				break;
			case PvP:
				enemyOption = PvEAI;
				label = "easy AI";
				break;
			}
			audioManager.PlayClick();
			this->mouseReleased = mouseReleased = 0;
		}
	}
}

//Time button
void Button::OnClick(int& mouseReleased, int& action)
{
	if (MouseOnButton())
	{
		if (mouseReleased)
		{
			switch (buttonType) {
			case TimeFormat:
				switch (action) {
				case 60:
					action = 30;
					label = "0:30";
					break;
				case 30:
					action = 10;
					label = "0:10";
					break;
				case 10:
					action = 60;
					label = "1:00";
					break;
				}
				break;
			case ChangeControlls:
				switch (action) {
				case 0:
					action = 1;
					label = "controller";
					break;
				case 1:
					action = 0;
					label = "keyboard";
					break;
				}
				break;
			}
			audioManager.PlayClick();
			this->mouseReleased = mouseReleased = 0;
		}
	}
}

bool Button::MouseOnButton()
{
	if ((mouseX >= x1) && (mouseX <= x2) && (mouseY <= y1) && (mouseY >= y2))
	{
		return true;
	}
	else return false;
}
