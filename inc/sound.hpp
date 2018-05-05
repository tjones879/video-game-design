#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum class SOUND_TYPE : char {
    MUSIC,
    EFFECT
};

class Sound {
public:
    //Class constructor and destructor
    Sound(std::string in_filename, SOUND_TYPE type);
    ~Sound();
    bool loadMusic(std::string in_filename);
    bool loadEffect(std::string in_filename);
    void playSound(int left);
private:
    SOUND_TYPE soundType;
    Mix_Music* soundMusic;
    Mix_Chunk* soundEffect;
};

class SoundManager {
public:
    SoundManager();
    ~SoundManager();
    std::shared_ptr<Sound> addSound(std::string in_filename, SOUND_TYPE type);
private:
    std::vector<std::shared_ptr<Sound>> sounds;
};
