#include "inc/sound.hpp"
#define WAV_PATH "assets/scratch.wav"
#define DEBUG(e) std::cerr << e << std::endl;
vector<Sound*> Sound::sounds;

Sound::Sound(string in_filename, SOUND_TYPE in_type)
{
	//Store the instance in the sounds vector
	sounds.push_back(this);

	soundType = in_type;

	if(soundType == SOUND_MUSIC)
	{
		if(loadMusic(in_filename) == false)
			exit(2);
	}

	if(soundType == SOUND_EFFECT)
	{
		if(loadEffect(in_filename) == false)
			exit(2);
	}
}

Sound::~Sound()
{
	//Delete initialized sound
	for (vector<Sound*>::iterator it = sounds.begin(); it != sounds.end(); it++)
	{
		if(this->soundType == SOUND_MUSIC)
		{
			Mix_FreeMusic(soundMusic);
		}
		else if(this->soundType == SOUND_EFFECT)
		{
			Mix_FreeChunk(soundEffect);
		}

		if((*it) == this)
			sounds.erase(it);
	}
}

void Sound::init()
{
	//Init SDL_mixer
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cout << "Error in Mix_OpenAudio" << std::endl;
        exit(4);
    }
}

bool Sound::loadMusic(string in_filename)
{
	soundMusic = Mix_LoadMUS( in_filename.c_str() );

	if(soundMusic == NULL)
		return false;
	else
		return true;
}

bool Sound::loadEffect (string in_filename)
{
	soundEffect = Mix_LoadWAV( in_filename.c_str() );

	if(soundEffect == NULL)
		return false;
	else
		return true;
}

void Sound::playSound(int left)
{
	if(soundType == SOUND_MUSIC)
	{
		//If there is no music playing
		if( Mix_PlayingMusic() == 0 )
		{
			//play the music
			Mix_VolumeMusic(60);
			Mix_FadeInMusic( soundMusic, -1, 6000 );
		}
		//if music is being played
		else
		{
			//if the music is paused
			if( Mix_PausedMusic() == 1 )
			{
				//resume the music
				Mix_ResumeMusic();
			}
			//if the music is playing
			else
			{
				//pause the music
				Mix_PauseMusic();
			}
		}
	}

	if(soundType == SOUND_EFFECT)
	{

		Mix_PlayChannel( MIX_DEFAULT_CHANNELS, soundEffect, 0 );
		// Sets appropriate volume for left and right channels
		Mix_SetPanning( MIX_DEFAULT_CHANNELS, left, 254 - left);

	}
}