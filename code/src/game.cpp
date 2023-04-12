#include "game.h"


namespace Tmpl8
{
	void Game::Init()
	{
		goal1.SetPosition({ 50, ScreenHeight - 32 });
		goal2.SetPosition({ ScreenWidth - 52, ScreenHeight - 32 });

		backgroundLayer->LoadImages("assets/Background.png");
	}

	void Game::Shutdown()
	{
	}

	void Game::Tick(float deltaTime)
	{
		deltaTime /= 1000.0f;

		switch (gameState)
		{
		case Button::Menu:
		{
			countdownTimer.reset();

			DrawMenu();

			//mouseReleased can only be true while hovering over one of the buttons
			if (!(startButton.MouseOnButton() || settingsButton.MouseOnButton() || infoButton.MouseOnButton()))
			{
				mouseReleased = 0;
			}

			startButton.OnClick(mouseReleased, gameState);		//gameState == Start
			settingsButton.OnClick(mouseReleased, gameState);	//gameState == Settings
			infoButton.OnClick(mouseReleased, gameState);		//gameState == InfoScreen

			audioManager.StopBackground();
		}
		break;
		case Button::Settings:
		{
			DrawSettings();

			//mouseReleased can only be true while hovering over one of the buttons
			if (!(backButton.MouseOnButton() || opponentButton.MouseOnButton() || timeButton.MouseOnButton()))
			{
				mouseReleased = 0;
			}

			backButton.OnClick(mouseReleased, gameState);		//gameState == Menu
			timeButton.OnClick(mouseReleased, matchLength);		//Change time format
			opponentButton.OnClick(mouseReleased, enemyOption);	//Change opponent
		}
		break;
		case Button::InfoScreen:
		{
			DrawInfoScreen();

			//mouseReleased can only be true while hovering over one of the buttons
			if (!(backButton2.MouseOnButton() || controlsButton.MouseOnButton()))
			{
				mouseReleased = 0;
			}

			backButton2.OnClick(mouseReleased, gameState);			//gameState == Menu
			controlsButton.OnClick(mouseReleased, isController);	//Switch information between keyboard & controller
		}
		break;
		//Called when starting or restarting a match
		case Button::Start:
		{
			gameTimer.reset();
			countdownTimer.reset();

			audioManager.PlayBackground();

			//Reset characters & ball
			EntityReset();
			ResetScore();

			countdownTimes = 0;
			IsStarted = false;
			matchStart = false;
			matchEnd = false;

			gameState = Button::Countdown;
		}
		break;
		//Called when a goal is scored
		case Button::Reset:
		{
			countdownTimer.reset();
			EntityReset();

			IsStarted = false;

			gameState = Button::Countdown;
		}
		break;
		case Button::Countdown:
		{
			countdownTimer.tick();

			Draw();
			Countdown();

			if (IsStarted)
			{
				//Limits the starting wistle to only play at the first countdown
				if (!matchStart) 
				{
					audioManager.PlayStart();
					matchStart = true;
				}
				gameState = Button::Playing;
			}
		}
		break;
		case Button::Playing:
		{
			DoPlaying(deltaTime);
		}
		break;
		}
	}

	void Game::Scored()
	{
		/***************************************************************

			Checks if there has been scored. If so,
			increment the character score and set gameState to Reset.

		****************************************************************/

		if (goal1.IsGoal(ball))
		{
			switch (enemyOption) 
			{
			case Button::PvP: 
				player2.SetScore(player2.GetScore() + 1); 
				break;
			case Button::PvEAI: 
				easyAI.SetScore(easyAI.GetScore() + 1); 
				break;
			case Button::PvHAI: 
				hardAI.SetScore(hardAI.GetScore() + 1); 
				break;
			}

			gameState = Button::Reset;
		}

		if (goal2.IsGoal(ball))
		{
			player.SetScore(player.GetScore() + 1);

			gameState = Button::Reset;
		}
	}

	void Game::DrawMenu()
	{
		screen->Clear(menuColor);

		screen->Print("Head soccer", 175, 100, 0xffffff, 10);

		startButton.Draw(screen, true);
		settingsButton.Draw(screen, true);
		infoButton.Draw(screen, true);
	}

	void Game::DrawSettings()
	{
		screen->Clear(menuColor);

		backButton.Draw(screen, true);

		screen->Print("Time format:", 315, 100, textColor, 3);
		timeButton.Draw(screen, true);

		screen->Print("Players vs", 340, 205, textColor, 3);
		opponentButton.Draw(screen, true);
	}

	void Game::DrawScoreboard()
	{
		const char* points[] = { "0","1","2","3","4","5","6","7","8","9" };

		int textWidth = 8;
		Pixel Color = 0xdf0000; //RED

		//Score Player 1:
		int score1 = player.GetScore();
		int hundreds_Player1 = score1 / 100;
		int tens_Player1 = (score1 >= 100) ? (score1 / 10) % 10 : score1 / 10;
		int units_Player1 = score1 % 10;
		//

		//Score Player 2 / AI:
		int score2 = (enemyOption == Button::PvP) ? player2.GetScore() : (enemyOption == Button::PvHAI) ? hardAI.GetScore() : easyAI.GetScore();
		int hundreds_Player2 = score2 / 100;
		int tens_Player2 = (score2 >= 100) ? (score2 / 10) % 10 : tens_Player2 = score2 / 10;
		int units_Player2 = score2 % 10;
		//

		// X positions Player 1:

		/* P1 score < 10 */
		int only_Units_1 = 257;

		/* P1 score >= 10 && < 100 */
		int units_For_Tens_1 = 275;
		int tens_For_Tens_1 = 230;

		/* P1 score >= 100 */
		int units_For_Hundreds_1 = 295;
		int tens_For_Hundreds_1 = 250;
		int hundreds_1 = 210;
		//

		// X positions Player 2 / AI:

		/* P2 score < 10 */
		int only_Units_2 = 707;

		/* P2 score >= 10 && < 100 */
		int units_For_Tens_2 = 725;
		int tens_For_Tens_2 = 680;

		/* P2 score >= 100 */
		int units_For_Hundreds_2 = 745;
		int tens_For_Hundreds_2 = 700;
		int hundreds_2 = 660;
		//

		// Y position
		int height = 100;

		/************************************************************************

			Draws & updates the numbers on the scoreboards, up to 999.

			The switch cases check if the player 1 & player 2 scores contain
			1, 2 or 3 integers, and based on that, draws them on the screen.

		*************************************************************************/

		switch (score1 > 99 ? 1 : score1 > 9 ? 2 : 3) {
		case 1:
			switch (score2 > 99 ? 1 : score2 > 9 ? 2 : 3) {
			case 1:
				screen->Print(points[hundreds_Player1], hundreds_1, height, Color, textWidth);
				screen->Print(points[tens_Player1], tens_For_Hundreds_1, height, Color, textWidth);
				screen->Print(points[units_Player1], units_For_Hundreds_1, height, Color, textWidth);

				screen->Print(points[hundreds_Player2], hundreds_2, height, Color, textWidth);
				screen->Print(points[tens_Player2], tens_For_Hundreds_2, height, Color, textWidth);
				screen->Print(points[units_Player2], units_For_Hundreds_2, height, Color, textWidth);
				break;
			case 2:
				screen->Print(points[hundreds_Player1], hundreds_1, height, Color, textWidth);
				screen->Print(points[tens_Player1], tens_For_Hundreds_1, height, Color, textWidth);
				screen->Print(points[units_Player1], units_For_Hundreds_1, height, Color, textWidth);

				screen->Print(points[tens_Player2], tens_For_Tens_2, height, Color, textWidth);
				screen->Print(points[units_Player2], units_For_Tens_2, height, Color, textWidth);
				break;
			case 3:
				screen->Print(points[hundreds_Player1], hundreds_1, height, Color, textWidth);
				screen->Print(points[tens_Player1], tens_For_Hundreds_1, height, Color, textWidth);
				screen->Print(points[units_Player1], units_For_Hundreds_1, height, Color, textWidth);

				screen->Print(points[units_Player2], only_Units_2, height, Color, textWidth);
				break;
			}
			break;
		case 2:
			switch (score2 > 99 ? 1 : score2 > 9 ? 2 : 3) {
			case 1:
				screen->Print(points[tens_Player1], tens_For_Tens_1, height, Color, textWidth);
				screen->Print(points[units_Player1], units_For_Tens_1, height, Color, textWidth);

				screen->Print(points[hundreds_Player2], hundreds_2, height, Color, textWidth);
				screen->Print(points[tens_Player2], tens_For_Hundreds_2, height, Color, textWidth);
				screen->Print(points[units_Player2], units_For_Hundreds_2, height, Color, textWidth);
				break;

			case 2:
				screen->Print(points[tens_Player1], tens_For_Tens_1, height, Color, textWidth);
				screen->Print(points[units_Player1], units_For_Tens_1, height, Color, textWidth);

				screen->Print(points[tens_Player2], tens_For_Tens_2, height, Color, textWidth);
				screen->Print(points[units_Player2], units_For_Tens_2, height, Color, textWidth);
				break;

			case 3:
				screen->Print(points[tens_Player1], tens_For_Tens_1, height, Color, textWidth);
				screen->Print(points[units_Player1], units_For_Tens_1, height, Color, textWidth);

				screen->Print(points[units_Player2], only_Units_2, height, Color, textWidth);
				break;
			}
			break;

		case 3:
			switch (score2 > 99 ? 1 : score2 > 9 ? 2 : 3) {
			case 1:
				screen->Print(points[units_Player1], only_Units_1, height, Color, textWidth);

				screen->Print(points[hundreds_Player2], hundreds_2, height, Color, textWidth);
				screen->Print(points[tens_Player2], tens_For_Hundreds_2, height, Color, textWidth);
				screen->Print(points[units_Player2], units_For_Hundreds_2, height, Color, textWidth);
				break;

			case 2:
				screen->Print(points[units_Player1], only_Units_1, height, Color, textWidth);

				screen->Print(points[tens_Player2], tens_For_Tens_2, height, Color, textWidth);
				screen->Print(points[units_Player2], units_For_Tens_2, height, Color, textWidth);
				break;

			case 3:
				screen->Print(points[units_Player1], only_Units_1, height, Color, textWidth);

				screen->Print(points[units_Player2], only_Units_2, height, Color, textWidth);
				break;
			}
			break;
		}
	}

	void Game::DrawGameTimer()
	{
		/******************************************************************************************************************************************

			elapsedSeconds is the total elapsed seconds since the beginning of the match, minus 3 * countdownTimes.
			This is because the gameTimer keeps running while the countdown is going on. Since the countdown is 3 seconds long,
			subtracting the total amount of seconds, by 3 * the amount of times the countdown has been invoked, gives the in-game playing timer.

			The result is then casted to an integer, so the elapsedSeconds stays the same variable for a second long.

		*******************************************************************************************************************************************/

		elapsedSeconds = static_cast<int>(gameTimer.totalSeconds() - (3 * countdownTimes));
		timeLeft = matchLength - elapsedSeconds;

		const char* points[] = { "0","1","2","3","4","5","6","7","8","9" };

		int textWidth = 6;

		int tens = timeLeft / 10;
		int units = timeLeft % 10;

		int height = 50;

		int Timer_X = 435;

		Pixel red = 0xff0000;
		Pixel white = 0xffffff;

		//If time <= 10 seonds, flicker red, otherwise color is white. If time <= 0, color is red.
		Pixel color = (timeLeft <= 10) ? timeLeft <= 0 ? red : (timeLeft % 2) == 0 ? red : white : white;

		/***************************************************************************************

			Draws & updates the numbers on the in-game timer, based on the variable timeLeft.

		****************************************************************************************/

		if (timeLeft == 60)
		{
			screen->Print("1", Timer_X, height, color, textWidth);
			screen->Print(":", Timer_X + 30, height - 5, color, textWidth);
			screen->Print("0", Timer_X + 60, height, color, textWidth);
			screen->Print("0", Timer_X + 100, height, color, textWidth);
		}
		else if ((timeLeft < 60) && (timeLeft > 0))
		{
			screen->Print("0", Timer_X, height, color, textWidth);
			screen->Print(":", Timer_X + 30, height - 5, color, textWidth);
			screen->Print(points[tens], Timer_X + 60, height, color, textWidth);
			screen->Print(points[units], Timer_X + 100, height, color, textWidth);
		}
		else if (timeLeft <= 0)
		{
			screen->Print("0", Timer_X, height, color, textWidth);
			screen->Print(":", Timer_X + 30, height - 5, color, textWidth);
			screen->Print("0", Timer_X + 60, height, color, textWidth);
			screen->Print("0", Timer_X + 100, height, color, textWidth);
		}
	}

	void Game::DrawWinScreen()
	{
		menuButton.Draw(screen, false);
		resetButton.Draw(screen, false);

		int textwidth = 10;

		int score1 = player.GetScore();
		int score2 = (enemyOption == Button::PvP) ? player2.GetScore() : (enemyOption == Button::PvHAI) ? hardAI.GetScore() : easyAI.GetScore();

		const char* winMessage_Player = "Player 1 WON!";
		const char* loseMessage_Player = "Player 2 WON!";
		const char* winMessage_AI = "YOU WIN!";
		const char* loseMessage_AI = "YOU LOSE...";
		const char* drawMessage = "ITS A DRAW";

		/*******************************************************************************************

			Draws a message, the menu & reset buttons, and sets the end frames for the characters.

			What message goes through, depends on the opponent type (and, obviously, the winner).

			frame 6 == happy face
			frame 7 == sad face

		********************************************************************************************/

		switch ((score1 > score2) ? 1 : (score1 < score2) ? 2 : 3) {
		case 1: //Player 1 won
			player.SetFrameFloat(6);

			//Play end-sound only once
			if (!matchEnd) 
			{
				audioManager.PlayWin(); 
				matchEnd = true;
			}

			if (enemyOption == Button::PvP)
			{
				screen->Print(winMessage_Player, ScreenWidth / 2 - (textwidth * static_cast<int>(strlen(winMessage_Player)) * 3), ScreenHeight / 2, 0xffdf00, textwidth);
				player2.SetFrameFloat(7);
			}
			else
			{
				screen->Print(winMessage_AI, ScreenWidth / 2 - (textwidth * static_cast<int>(strlen(winMessage_AI)) * 3), ScreenHeight / 2, 0xffdf00, textwidth);

				switch (enemyOption) 
				{
				case Button::PvEAI: 
					easyAI.SetFrameFloat(7); 
					break;
				case Button::PvHAI: 
					hardAI.SetFrameFloat(7); 
					break;
				}
			}
			break;
		case 2: //Player 1 lost
			player.SetFrameFloat(7);

			//Play end-sound only once
			if (!matchEnd)
			{
				audioManager.PlayLose();
				matchEnd = true;
			}

			if (enemyOption == Button::PvP)
			{
				screen->Print(loseMessage_Player, ScreenWidth / 2 - (textwidth * static_cast<int>(strlen(loseMessage_Player)) * 3), ScreenHeight / 2, 0x0000c0, textwidth);
				player2.SetFrameFloat(6);
			}
			else
			{
				screen->Print(loseMessage_AI, ScreenWidth / 2 - (textwidth * static_cast<int>(strlen(loseMessage_AI) * 2.5f)), ScreenHeight / 2, 0x0000c0, textwidth);
				switch (enemyOption)
				{
				case Button::PvEAI:
					easyAI.SetFrameFloat(6);
					break;
				case Button::PvHAI:
					hardAI.SetFrameFloat(6);
					break;
				}
			}
			break;
		case 3: //Draw

			//Play end-sound only once
			if (!matchEnd)
			{
				audioManager.PlayLose();
				matchEnd = true;
			}

			screen->Print(drawMessage, ScreenWidth / 2 - (textwidth * static_cast<int>(strlen(drawMessage)) * 3), ScreenHeight / 2, 0xffdf00, textwidth);
			break;
		}
	}

	void Game::DrawInfoScreen()
	{
		//(x1, y1) is the bottomleft corner
		float x1 = 0;
		float y1 = ScreenHeight;

		//(x2, y2) is the topright corner
		float x2 = ScreenWidth;
		float y2 = 0;

		Tmpl8::vec2 middle{ x1 + ((x2 - x1) / 2), y1 + ((y2 - y1) / 2) };

		int textWidth = 3;

		int offset = 25;

		const char* text2 = "player 1 controls:";
		const char* text3 = "up:  w/space";
		const char* text4 = "left:      a";
		const char* text5 = "right:     d";

		const char* text6 = "Player 2 controls:";
		const char* text7 = "arrow keys";

		const char* text9 = "up:  a button";
		const char* text10 = "left:  left stick";
		const char* text11 = "right: left stick";

		/*************************************************************************

			Draws the controls information, and the back & controls buttons.

			isController == 0, is the keyboard information.
			isController == 1, is the controller information.

		**************************************************************************/

		screen->Clear(menuColor);

		backButton2.Draw(screen, true);

		controlsButton.Draw(screen, true);

		if (isController == 0)
		{
			screen->Print(text2, static_cast<int>(middle.x - (textWidth * strlen(text2) * 3)), 100 + offset, textColor, textWidth);
			screen->Print(text3, static_cast<int>(middle.x - (textWidth * strlen(text3) * 3)), 150 + offset, textColor, textWidth);
			screen->Print(text4, static_cast<int>(middle.x - (textWidth * strlen(text4) * 3)), 200 + offset, textColor, textWidth);
			screen->Print(text5, static_cast<int>(middle.x - (textWidth * strlen(text5) * 3)), 250 + offset, textColor, textWidth);
			screen->Print(text6, static_cast<int>(middle.x - (textWidth * strlen(text6) * 3)), 350 + offset, textColor, textWidth);
			screen->Print(text7, static_cast<int>(middle.x - (textWidth * strlen(text7) * 3)), 400 + offset, textColor, textWidth);
		}

		if (isController == 1)
		{
			screen->Print(text9, static_cast<int>(middle.x - (textWidth * strlen(text9) * 3)), 100 + offset, textColor, textWidth);
			screen->Print(text10, static_cast<int>(middle.x - (textWidth * strlen(text10) * 3)), 150 + offset, textColor, textWidth);
			screen->Print(text11, static_cast<int>(middle.x - (textWidth * strlen(text11) * 3)), 200 + offset, textColor, textWidth);
		}
	}

	void Game::Countdown()
	{
		int textwidth = 25;

		float totalTime = static_cast<float>(countdownTimer.totalSeconds());

		const char* num[] = { "1","2","3" };

		int waitTime = 3;

		float countdownTimeLeft = waitTime - totalTime; //Seconds 

		/*****************************************************************************************

			If countdownTimeLeft is bigger than 0, display it on the screen.
			Else, reset the countdownTimer, increment countdownTimes and set isStarted to true.

		******************************************************************************************/

		if (countdownTimeLeft > 0)
		{
			screen->Print(num[static_cast<int>(countdownTimeLeft)], 435, 150, 0xffdf00, textwidth);
		}
		else
		{
			countdownTimer.reset();
			countdownTimes++;
			IsStarted = true;
		}
	}

	void Game::DoPlaying(float deltaTime)
	{
		/*********************************************************************************

			The game loop.

			Updates and draws all the entities to the screen.

			While timeLeft > 0, check for goals.
			While timeLeft <= 0, draw the winscreen, and make the moving entities stop.

		**********************************************************************************/

		gameTimer.tick();

		//MouseReleased can only be true while hovering over one of the buttons
		if (!(resetButton.MouseOnButton() || menuButton.MouseOnButton()))
		{
			mouseReleased = 0;
		}


		Draw();
		Update(deltaTime);
		Frames(deltaTime);
		EntityCollision();

		//During the match
		if (timeLeft > 0)
		{
			Scored();
			EntityMove();

			/**********************************************************************************
		 
				When a character hits the ball, play the headbutt-sound. 
				If the last character that hit the ball is the same character being hit now, 
				play the audio queued instead. This is to prevent the sound from 
				playing over and over if the ball clips into the player.

			***********************************************************************************/
			if (player.GetHit())
			{
				if (whatPlayer) audioManager.PlayHeadbutt();
				else audioManager.PlayHeadbuttQueued();
				whatPlayer = 0;

				if (controller1.IsConnected()) controller1.SetVibration(30);
				player.setHit(0);
			}

			switch (enemyOption)
			{
			case Button::PvP:
				if (player2.GetHit())
				{
					if (!whatPlayer) audioManager.PlayHeadbutt();
					else audioManager.PlayHeadbuttQueued();
					whatPlayer = 1;

					if (controller2.IsConnected()) controller2.SetVibration(30);

					player2.setHit(0);
				}
				break;
			case Button::PvEAI:
				if (easyAI.GetHit())
				{
					if (!whatPlayer) audioManager.PlayHeadbutt();
					else audioManager.PlayHeadbuttQueued(); 
					whatPlayer = 1;

					if (controller2.IsConnected()) controller2.SetVibration(30);

					easyAI.setHit(0); 
				}
				break;
			case Button::PvHAI:
				if (hardAI.GetHit())
				{
					if (!whatPlayer) audioManager.PlayHeadbutt();
					else audioManager.PlayHeadbuttQueued(); 
					whatPlayer = 1;

					if (controller2.IsConnected()) controller2.SetVibration(30);

					hardAI.setHit(0); 
				}
				break;
			}
		}
		//After the match
		else
		{
			//Play the end wistle-sound. In DrawWinscreen(), matchEnd is set to 1, so this sound only plays once.
			if (!matchEnd)
			{
				audioManager.PlayEnd();
			}

			DrawWinScreen();
			StopEntities();

			menuButton.OnClick(mouseReleased, gameState);
			resetButton.OnClick(mouseReleased, gameState);
		}

	}

	void Game::EntityReset()
	{
		//Reset moving entities
		player.Reset(200, P1_Sprite);

		switch (enemyOption) 
		{
		case Button::PvP: 
			player2.Reset(800, P2_Sprite); 
			break;
		case Button::PvEAI: 
			easyAI.Reset(800, P2_Sprite); 
			break;
		case Button::PvHAI: 
			hardAI.Reset(800, P2_Sprite); 
			break;
		}

		ball.Reset(ballSprite);
	}

	void Game::Draw()
	{
		/********************************************
			Draw everything for the playing-state.
		*********************************************/

		backgroundLayer->CopyTo(screen, 0, 0);

		DrawScoreboard();
		DrawGameTimer();
		
		switch (enemyOption) 
		{
		case Button::PvP: 
			P2_Sprite.DrawScaled(static_cast<int>(player2.GetPosition().x) - 60, static_cast<int>(player2.GetPosition().y) - 100, 120, 120, screen);
			break;
		case Button::PvEAI: 
			P2_Sprite.DrawScaled(static_cast<int>(easyAI.GetPosition().x) - 60, static_cast<int>(easyAI.GetPosition().y) - 100, 120, 120, screen);
			break;
		case Button::PvHAI: 
			P2_Sprite.DrawScaled(static_cast<int>(hardAI.GetPosition().x) - 60, static_cast<int>(hardAI.GetPosition().y) - 100, 120, 120, screen);
			break;
		}

		P1_Sprite.DrawScaled(static_cast<int>(player.GetPosition().x) - 60, static_cast<int>(player.GetPosition().y) - 100, 120, 120, screen);
		ballSprite.Draw(screen, static_cast<int>(ball.GetPosition().x) - 33, static_cast<int>(ball.GetPosition().y) - 34);

		netSprite.Draw(screen, 0, 0);
	}

	void Game::ResetScore()
	{
		player.SetScore(0);

		switch (enemyOption) 
		{
		case Button::PvP: 
			player2.SetScore(0); 
			break;
		case Button::PvEAI: 
			easyAI.SetScore(0); 
			break;
		case Button::PvHAI: 
			hardAI.SetScore(0); 
			break;
		}
	}

	void Game::Update(float deltaTime)
	{
		/**************************************************
			Updates the physics for the moving entities,
			and the state & vibrations of the controller.
		***************************************************/

		ball.Update(deltaTime);
		player.Update(deltaTime);
		switch (enemyOption) 
		{
		case Button::PvP: 
			player2.Update(deltaTime); 
			break;
		case Button::PvEAI: 
			easyAI.Update(deltaTime * 0.8f);  
			break;
		case Button::PvHAI:
			hardAI.Update(deltaTime);
			break;
		}

		controller1.Update(deltaTime);
		controller2.Update(deltaTime);
	}

	void Game::EntityCollision()
	{
		/****************************
			All entity collisions.
		*****************************/

		goal1.BallCollision(ball);
		goal2.BallCollision(ball);

		ball.WallCollision();

		player.WallCollision();
		if (timeLeft > 0) player.BallCollision(ball);
		player.GoalCollision();

		switch (enemyOption) 
		{
		case Button::PvP: 
			player2.WallCollision(); 
			if (timeLeft > 0) player2.BallCollision(ball); 
			player2.GoalCollision(); 
			break;
		case Button::PvEAI:
			easyAI.WallCollision(); 
			if (timeLeft > 0) easyAI.BallCollision(ball); 
			easyAI.GoalCollision(); 
			break;
		case Button::PvHAI: 
			hardAI.WallCollision(); 
			if (timeLeft > 0) hardAI.BallCollision(ball); 
			hardAI.GoalCollision(); 
			break;
		}
	}

	void Game::EntityMove()
	{
		player.Move();

		switch (enemyOption) 
		{
		case Button::PvP: 
			player2.Move(); ///P2 
			break;
		case Button::PvEAI: 
			easyAI.Move(ball); ///EasyAI 
			break;
		case Button::PvHAI: 
			hardAI.Move(ball); ///HardAI
			break;
		}
	}

	void Game::StopEntities()
	{
		//Stop players from moving
		player.SetVelocity(vec2{ 0 });
		player.SetGravity(vec2{ 0, 500 });

		switch (enemyOption)
		{
		case Button::PvP: 
			player2.SetVelocity(vec2{ 0 }); 
			player2.SetGravity(vec2{ 0, 500 }); 
			break;
		case Button::PvEAI: 
			easyAI.SetVelocity(vec2{ 0 }); 
			easyAI.SetGravity(vec2{ 0, 500 }); 
			break;
		case Button::PvHAI: 
			hardAI.SetVelocity(vec2{ 0 }); 
			hardAI.SetGravity(vec2{ 0, 500 }); 
			break;
		}

		ball.SetGravity({ ball.GetGravity().x, 25 });

		//Gradually decrease ball velocity
		if (ball.IsGround())
		{
			// X-velocity
			ball.SetVelocity({ static_cast<float>(ball.GetVelocity().x / 1.5) , ball.GetVelocity().y });

			// Y-velocity
			ball.SetVelocity({ ball.GetVelocity().x , static_cast<float>(ball.GetVelocity().y / 1.5) });
		}

		//When the X-velocity of the ball ~= 0, make the ball sprite static.
		if ((ball.GetVelocity().x <= 10) && (ball.GetVelocity().x >= -10))
		{
			ball.SetFrameFloat(0);
		}
	}

	void Game::Frames(float deltaTime)
	{
		/***********************************************
			Update the moving entities-sprite frames.
		************************************************/

		ball.Frames(deltaTime, ballSprite);
		player.Frames(deltaTime, P1_Sprite);

		switch (enemyOption) 
		{
		case Button::PvP: 
			player2.Frames(deltaTime, P2_Sprite); ///P2 
			break; 
		case Button::PvEAI: 
			easyAI.Frames(deltaTime * 0.8f, P2_Sprite); ///EasyAI
			break;
		case Button::PvHAI:
			hardAI.Frames(deltaTime, P2_Sprite); ///HardAI
			break;
		}
	}
};