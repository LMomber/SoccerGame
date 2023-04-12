#pragma once

///Based on code from Cynic, 27/03/2023
///Discord link: https://discord.com/channels/515453022097244160/913396868002762792/1089992771668029552
///Github:		 https://github.com/Cynic1254/Game/blob/main/AudioPlayer.cpp

#include "SDL.h"
#include <SDL_mixer.h>

//Audio player for sound effects
class SoundPlayer
{
public:
	SoundPlayer(const char* audio);

	void PlaySound() const;
	void PlaySoundQueued() const;

private:
	//The wave file's format details
	SDL_AudioSpec audioSpec_;
	//The audio data
	Uint8* audioBuffer_;
	//Length of audio data in bytes
	Uint32 audioLength_;

	SDL_AudioDeviceID deviceId_;
};


//Audio player for music / background sounds.
class MusicPlayer
{
public:
	MusicPlayer(const char* audio);

	void PlayMusic() const;
	void StopSound() const;

private:
	Mix_Music* music;
};