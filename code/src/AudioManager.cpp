#include "AudioManager.h"

void AudioManager::PlayHeadbutt() const
{
	Headbutt.PlaySound();
}

void AudioManager::PlayHeadbuttQueued() const
{
	Headbutt.PlaySoundQueued();
}

void AudioManager::PlayP1Score()
{
	switch (p1ScoreSoundRotation) {
	case 0:
		p1ScoreSound1.PlaySound();
		p1ScoreSoundRotation++;
		break;
	case 1:
		p1ScoreSound2.PlaySound();
		p1ScoreSoundRotation = 0;
		break;
	}
}

void AudioManager::PlayP2Score()
{
	switch (p2ScoreSoundRotation) {
	case 0:
		p2ScoreSound1.PlaySound();
		p2ScoreSoundRotation++;
		break;
	case 1:
		p2ScoreSound2.PlaySound();
		p2ScoreSoundRotation = 0;
		break;
	}
}

void AudioManager::PlayClick() const
{
	buttonSound.PlaySound();
}

void AudioManager::PlayWin() const
{
	winSound.PlaySound();
}

void AudioManager::PlayLose() const
{
	loseSound.PlaySound();
}

void AudioManager::PlayStart() const
{
	startSound.PlaySound();
}

void AudioManager::PlayEnd() const
{
	endSound.PlaySound();
}

void AudioManager::PlayBackground() const
{
	backgroundNoise.PlayMusic();
}

void AudioManager::StopBackground() const
{
	backgroundNoise.StopSound();
}
