#include "inc/sound.hpp"
#define WAV_PATH "assets/scratch.wav"
#define DEBUG(e) std::cerr << e << std::endl;

Sound::Sound(std::string inFilename, SOUND_TYPE type)
{
    soundType = type;
    if (soundType == SOUND_TYPE::MUSIC)
        if(!loadMusic(inFilename))
            exit(2);

    if (soundType == SOUND_TYPE::EFFECT)
        if(!loadEffect(inFilename))
            exit(2);
}

Sound::~Sound()
{
    if (soundType == SOUND_TYPE::MUSIC)
        Mix_FreeMusic(soundMusic);
    else
        Mix_FreeChunk(soundEffect);
}

bool Sound::loadMusic(std::string in_filename)
{
    soundMusic = Mix_LoadMUS(in_filename.c_str());

    return !!soundMusic;
}

bool Sound::loadEffect(std::string in_filename)
{
    soundEffect = Mix_LoadWAV(in_filename.c_str());

    return !!soundEffect;
}

void Sound::playSound(int left)
{
    if(soundType == SOUND_TYPE::MUSIC) {
        if(!Mix_PlayingMusic()) {
            Mix_VolumeMusic(60);
            Mix_FadeInMusic(soundMusic, -1, 6000);
        } else {
            if (Mix_PausedMusic())
                Mix_ResumeMusic();
            else
                Mix_PauseMusic();
        }
    } else {
        Mix_PlayChannel(MIX_DEFAULT_CHANNELS, soundEffect, 0);
        // Sets appropriate volume for left and right channels
        Mix_SetPanning(MIX_DEFAULT_CHANNELS, left, 254 - left);
    }
}

SoundManager::SoundManager()
{
    //Init SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cout << "Error in Mix_OpenAudio" << std::endl;
        exit(4);
    }
}

SoundManager::~SoundManager() {}

std::shared_ptr<Sound> SoundManager::addSound(std::string in_filename, SOUND_TYPE type)
{
    auto sound = std::make_shared<Sound>(in_filename, type);
    sounds.push_back(sound);
    return sound;
}

