#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <iostream>

namespace Tmpl8
{
	void Game::Init()
	{
		ball.SetPosition({ ScreenWidth / 2.0f, ScreenHeight / 2.0f });
		ball.SetVelocity(vec2{ 0 });
		ball.SetGravity(vec2{ 0, 17 }); //17
		ball.SetColor(BlueMask);

		player.SetPosition({ 200, ScreenHeight - 1 });
		player.SetVelocity(vec2{ 0 });
		player.SetGravity(vec2{ 0, 50 });
		player.SetColor(GreenMask);

		player2.SetPosition({ 600, ScreenHeight - 1 });
		player2.SetVelocity(vec2{ 0 });
		player2.SetGravity(vec2{ 0, 50 });
		player2.SetColor(RedMask);

		opponent.SetPosition({ 800, ScreenHeight - 1 });
		opponent.SetVelocity(vec2{ 0 });
		opponent.SetGravity(vec2{ 0, 50 });
		opponent.SetColor(RedMask);

		goal1.SetPosition({ 50, ScreenHeight });
		goal2.SetPosition({ ScreenWidth - 52, ScreenHeight });
	}

	void Game::Shutdown()
	{
	}

	void Game::Tick(float deltaTime)
	{
		deltaTime /= 1000.0f;

		ball.Update(deltaTime);
		player.Update(deltaTime);
		//player2.Update(deltaTime);

		controller1.Update(deltaTime);
		controller2.Update(deltaTime);

		goal1.Update(ball);
		goal2.Update(ball);

		opponent.Update(deltaTime);

		if (player.Collision(ball)) controller1.SetVibration(30);
		//if (player2.Collision(ball)) controller2.SetVibration(30);

		ball.WallCollision();

		player.Move(ball, 0x41, 0x44, 0x57, controller1, controller1.GetThumbStickL(), controller1.GetButtonA()); // A, D, SPACE
		//player2.Move(ball, VK_LEFT, VK_RIGHT, VK_UP, controller2, controller2.GetThumbStickL(), controller2.GetButtonA());

		opponent.Move(ball);

		screen->Clear(0);

		screen->Line(0, 200, ScreenWidth, 200, 0x00ff00);
		screen->Line(0, 350, ScreenWidth, 350, 0x00ff00); // or 330

		/*screen->Line(100, 0, 100, ScreenHeight, 0x00ff00);								//goal 1
		screen->Line(ScreenWidth - 100, 0, ScreenWidth - 100, ScreenHeight, 0x00ff00);	//goal 2
		screen->Line(500, 0, 500, ScreenHeight, 0x00ff00);								//mid_distance
		screen->Line(800, 0, 800, ScreenHeight, 0x00ff00);								//keepers_distance */

		goal1.Draw(screen);
		goal2.Draw(screen);
		ball.Draw(screen);
		player.Draw(screen);
		//player2.Draw(screen);

		opponent.Draw(screen);

		GoalCheck();
		Scoreboard();
	}

	void Game::GoalCheck()
	{
		if (!isGoal && goal1.Scored(ball))
		{
			isGoal = true;
			player2.SetScore(player2.GetScore() + 1);
		}

		if (!isGoal && goal2.Scored(ball))
		{
			isGoal = true;
			player.SetScore(player.GetScore() + 1);
		}

		isGoal = goal1.Scored(ball) || goal2.Scored(ball);
	}

	void Game::Scoreboard()
	{
		const char* points[] = { "0","1","2","3","4","5","6","7","8","9" };

		int textScale = 8;
		Pixel white = 0xffffff;

		int rounded_Down1 = player.GetScore() / 10;
		int rounded_Remainder1 = player.GetScore() % 10;

		int rounded_Down2 = player2.GetScore() / 10;
		int rounded_Remainder2 = player2.GetScore() % 10;

		int tens_Pos1 = static_cast<int>(ScreenWidth / 4.0f - (textScale * 2));
		int units_Pos1 = static_cast<int>(ScreenWidth / 4.0f + (textScale * 3.5f));

		int tens_Pos2 = static_cast<int>((ScreenWidth / 4.0f) * 3.0f - (textScale * 2));
		int units_Pos2 = static_cast<int>((ScreenWidth / 4.0f) * 3.0f + (textScale * 3.5f));

		int semicolon_Pos = static_cast<int>(ScreenWidth / 2 - (textScale * 2));

		int height = 100;

		if (player.GetScore() > 9 && player2.GetScore() > 9)
		{
			screen->Print(points[rounded_Down1], tens_Pos1, height, white, textScale);
			screen->Print(points[rounded_Remainder1], units_Pos1, height, white, textScale);
			screen->Print(":", semicolon_Pos, height, white, textScale);
			screen->Print(points[rounded_Down2], tens_Pos2, height, white, textScale);
			screen->Print(points[rounded_Remainder2], units_Pos2, height, white, textScale);
		}
		else if (player.GetScore() > 9 && player2.GetScore() <= 9)
		{
			screen->Print(points[rounded_Down1], tens_Pos1, height, white, textScale);
			screen->Print(points[rounded_Remainder1], units_Pos1, height, white, textScale);
			screen->Print(":", semicolon_Pos, height, white, textScale);
			screen->Print(points[player2.GetScore()], tens_Pos2, height, white, textScale);
		}
		else if (player.GetScore() <= 9 && player2.GetScore() > 9)
		{
			screen->Print(points[player.GetScore()], tens_Pos1, height, white, textScale);
			screen->Print(":", semicolon_Pos, height, white, textScale);
			screen->Print(points[rounded_Down2], tens_Pos2, height, white, textScale);
			screen->Print(points[rounded_Remainder2], units_Pos2, height, white, textScale);
		}
		else if (player.GetScore() <= 9 && player2.GetScore() <= 9)
		{
			screen->Print(points[player.GetScore()], tens_Pos1, height, white, textScale);
			screen->Print(":", semicolon_Pos, height, white, textScale);
			screen->Print(points[player2.GetScore()], tens_Pos2, height, white, textScale);
		}
	}
};