#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

enum SOUND_TYPE
{
	SOUND_MUSIC,
	SOUND_EFFECT
};

class Sound
{
public:
	//Class constructor and destructor
	Sound(string in_filename, SOUND_TYPE type);
	~Sound();

public:

	//Class functions

	//Initialize SDL_mixer (sound)
	static void init();
	//Load music
	bool loadMusic(string in_filename);
	//Load sound effect
	bool loadEffect(string in_filename);

	void playSound(int left);

	static vector<Sound*> sounds;

private:

	//Class member

	SOUND_TYPE soundType;
	Mix_Music* soundMusic;
	Mix_Chunk* soundEffect;

};