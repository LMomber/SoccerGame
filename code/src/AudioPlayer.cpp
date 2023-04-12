///Based on code from Cynic, 27/03/2023
///Discord link: https://discord.com/channels/515453022097244160/913396868002762792/1089992771668029552
///Github:		 https://github.com/Cynic1254/Game/blob/main/AudioPlayer.cpp

#include "AudioPlayer.h"
#include <iostream>

SoundPlayer::SoundPlayer(const char* audio)
{
	//Loads WAV from file. The audio data is stored in the memory adresses of the given pointers
	//If the function fails, an error is send to the console
	if (SDL_LoadWAV(audio, &audioSpec_, &audioBuffer_, &audioLength_) == NULL)
	{
		std::cout << "Could not open " << audio << ": " << SDL_GetError() << std::endl;
	}

	//Gives the audio file a unique ID, ranging from 2 to 16.
	deviceId_ = SDL_OpenAudioDevice(NULL, 0, &audioSpec_, NULL, 0);
	//std::cout << audio << ": " << deviceId_ << std::endl; 
}

void SoundPlayer::PlaySound() const
{
	/******************************************************************************
	
		Queues audio, and then plays it. 

		Since the queue shouldn't wait until the last audio fragment 
		is done before playing the new one, The audio queue gets cleared first.
	
	*******************************************************************************/

	SDL_ClearQueuedAudio(deviceId_);
	SDL_QueueAudio(deviceId_, audioBuffer_, audioLength_);

	//Pause function set to 0 == playing.
	SDL_PauseAudioDevice(deviceId_, 0);
}

void SoundPlayer::PlaySoundQueued() const
{
	/*********************************************************************************

		Plays audio queued.

		Because no more than 1 audio fragment should be queued at a time, 
		The function first checks if the amount of bytes waiting in the queue > 0.

	**********************************************************************************/

	if (SDL_GetQueuedAudioSize(deviceId_) <= 0)
	{
		SDL_QueueAudio(deviceId_, audioBuffer_, audioLength_);
		SDL_PauseAudioDevice(deviceId_, 0);
	}
}

/****************************************************************************************************/

MusicPlayer::MusicPlayer(const char* audio)
{
	//Load audio format into music object, in this case .ogg
	music = Mix_LoadMUS(audio);
	if (music == NULL) {
		std::cout << "Could not open " << music << ": " << SDL_GetError() << std::endl;
	}
}

void MusicPlayer::PlayMusic() const
{
	Mix_PlayMusic(music, -1);
}

void MusicPlayer::StopSound() const
{
	Mix_HaltMusic();
}
