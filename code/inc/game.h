#pragma once

#include "Ball.h"
#include "Player.h"
#include "AI.h"
#include "Goal.h"
#include "Timer.hpp"
#include "surface.h"
#include "Button.h"
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
		void MouseUp(int button) { mouseReleased = button; if (mouseReleased) mousePressed = 0; }
		void MouseDown(int button) { mousePressed = button; if (mousePressed) mouseReleased = 0; }
		void MouseMove(int x, int y) { mouseX = x, mouseY = y; }
		void KeyUp(int key = 0) { /* implement if you want to handle keys */ }
		void KeyDown(int key = 0) { /* implement if you want to handle keys */ }

		//If scored, increment the score
		void Scored();

		//Stop all moving entities
		void StopEntities();

		//Countdown functionalities
		void Countdown();

		//The playing-state loop
		void DoPlaying(float deltaTime);

		//Draw functions
		void DrawMenu();
		void DrawSettings();
		void DrawScoreboard();
		void DrawGameTimer();
		void DrawWinScreen();
		void DrawInfoScreen();

		//Collective functions (e.g. Draw() = Ball.Draw(), Player.Draw(), AI.Draw(), etc)
		void Draw();
		void Update(float deltaTime);
		void Frames(float deltaTime);
		void EntityCollision();
		void EntityMove();
		void EntityReset();
		void ResetScore();

	private:

		const Pixel labelColor = 0xffffff;
		const Pixel textColor = 0xffffff;
		const Pixel menuColor = 0x639bff; // Light blue

		//Constructors

		Surface* backgroundLayer = new Surface(ScreenWidth, ScreenHeight);
		Surface* screen;

		Ball ball;

		Controller controller1{ 0 };
		Controller controller2{ 1 };

		Player player{0x41, 0x44, 0x20, 0x57, controller1}; //A , D, W, Space, player
		Player player2{VK_LEFT, VK_RIGHT, VK_UP, controller2}; //Arrow keys, player

		AI hardAI{ 500, 650, 850 };
		AI easyAI{ 550, 600, 800 };

		Goal goal1{ 1 };
		Goal goal2{ 2 };

		AudioManager audioManager = AudioManager::getInstance();

		// Menu buttons
		Button startButton{ 400, 300, 200, 100, 0xffffff, "start", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::StartButton };
		Button infoButton{ ScreenWidth - 139, 448, 75, 75, 0xffffff, "?", mouseX, mouseY, mouseReleased, mousePressed, 8, labelColor, Button::Information };
		Button settingsButton{ 410, 448, 180, 75, 0xffffff, "settings", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::GoToSettings };

		// In-game buttons
		Button resetButton{ 400, 230, 200, 50, 0xffffff, "play again", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::StartButton };
		Button menuButton{ 400, 170, 200, 50, 0xffffff, "menu", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::BackToMenu };

		// Settings buttons
		Button backButton{ 75, 143, 150, 75, 0xffffff, "back", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::BackToMenu };
		Button opponentButton{ 540, 248,175, 75, 0xffffff, "hard AI", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::ChangeOpponent };
		Button timeButton{ 540, 143, 175, 75, 0xffffff, "1:00", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::TimeFormat };

		// Information screen button
		Button backButton2{ 75, 143, 150, 75, 0xffffff, "back", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::BackToMenu };
		Button controlsButton{ 400, 80, 190, 50, 0xffffff, "keyboard", mouseX, mouseY, mouseReleased, mousePressed, 3, labelColor, Button::ChangeControlls };

		Sprite P1_Sprite{ new Surface("assets/Player.png") , 8 };
		Sprite P2_Sprite{ new Surface("assets/Player2.png"), 8 };
		Sprite ballSprite{ new Surface("assets/BallSprite.png"), 2 };
		Sprite netSprite{ new Surface("assets/SoccerNet.png"), 1 };

		Graphics::Timer gameTimer;		//Used for the in-game timer
		Graphics::Timer countdownTimer; //Used for the countdown

		Button::State gameState{ Button::Menu }; 
		Button::Opponent enemyOption{ Button::PvHAI }; 
		//

		//Bools used for playing the starting/end wistle - sounds
		bool matchStart{ false };
		bool matchEnd{ false };

		//Variable for keeping track of which character hit the ball last.
		int whatPlayer{0};

		int elapsedSeconds;	   //Elapsed time
		int matchLength{ 60 }; //Match time length
		int timeLeft;		   //matchLength - elapsedSeconds
		int countdownTimes;    //Amount of times the countdown is used in a match

		//Integer for assigning controllers to the players
		int isController{ 0 };

		//Bool that is false during countdown, and true during the playing-state
		bool IsStarted = false;

		//Variables for registering mouse clicks
		int mouseX, mouseY;
		int mousePressed;
		int mouseReleased;
	};
}; // namespace Tmpl8