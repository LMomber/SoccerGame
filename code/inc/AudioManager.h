#pragma once

#include "AudioPlayer.h"

/**************************************
	A singleton audiomanager class
***************************************/
class AudioManager
{
public:
	static AudioManager& getInstance() {
		static AudioManager instance;
		return instance;
	}

	void PlayHeadbutt() const;
	void PlayHeadbuttQueued() const;
	void PlayP1Score();
	void PlayP2Score();
	void PlayClick() const;
	void PlayWin() const;
	void PlayLose() const;
	void PlayStart() const;
	void PlayEnd() const;

	void PlayBackground() const;
	void StopBackground() const;

private:
	AudioManager() {}

	SoundPlayer Headbutt{ "assets/Sounds/Headbutt.wav" };
	SoundPlayer p1ScoreSound1{ "assets/Sounds/Goal1.wav" };
	SoundPlayer p1ScoreSound2{ "assets/Sounds/Goal2.wav" };
	SoundPlayer p2ScoreSound1{ "assets/Sounds/Goal_enemy1.wav" };
	SoundPlayer p2ScoreSound2{ "assets/Sounds/Goal_enemy2.wav" };
	SoundPlayer buttonSound{ "assets/Sounds/Click2.wav" };
	SoundPlayer winSound{ "assets/Sounds/Win.wav" };
	SoundPlayer loseSound{ "assets/Sounds/Lose.wav" };
	SoundPlayer startSound{ "assets/Sounds/Start.wav" };
	SoundPlayer endSound{ "assets/Sounds/End.wav" };

	MusicPlayer backgroundNoise{ "assets/Sounds/BG1.ogg" };

	int p1ScoreSoundRotation{ 0 };
	int p2ScoreSoundRotation{ 0 };
};

