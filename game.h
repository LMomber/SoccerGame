#pragma once

#include "Ball.h"
#include "Player.h"
#include "Opponent.h"
#include "Goal.h"
#include "Controller.h"

namespace Tmpl8 {

	class Surface;
	class Game
	{
	public:
		void SetTarget(Surface* surface) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick(float deltaTime);
		void MouseUp(int button) { /* implement if you want to detect mouse button presses */ }
		void MouseDown(int button) { /* implement if you want to detect mouse button presses */ }
		void MouseMove(int x, int y) { /* implement if you want to detect mouse movement */ }
		void KeyUp(int key) { /* implement if you want to handle keys */ }
		void KeyDown(int key) { /* implement if you want to handle keys */ }
		void GoalCheck();
		void Scoreboard();
	private:
		Surface* screen;

		Ball ball; // default construct

		Player player;
		Player player2;

		Controller controller1{ 0 };
		Controller controller2{ 1 };

		Opponent opponent;

		Goal goal1{ 1 };
		Goal goal2{ 2 };

		bool isGoal = false;
	};

}; // namespace Tmpl8