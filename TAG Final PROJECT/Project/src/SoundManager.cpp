#include "SoundManager.h"



#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>


SoundManager::SoundManager() {}

SoundManager::~SoundManager() {}

/** Inititalizes the Sound Manager, loads all Sounds etc. **/
void SoundManager::init(){
    initSound();
}

/** Initializes the sound of the SDL Mixer **/
void SoundManager::initSound(void){
    // Init process from https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_11.html

    // start SDL with audio support
    if(SDL_Init(SDL_INIT_AUDIO)==-1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    // open 44.1KHz, signed 16bit, system byte order,
    //      stereo audio, using 1024 byte chunks
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024)==-1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(2);
    }

    // Load Footstep sound
    soundFootstep = Mix_LoadWAV(FILENAME_FOOTSTEP_SOUND);
    soundFootstep->volume = 25; // reduce volume of the footsteps
    soundChannelFootSteps = Mix_PlayChannel(soundChannelFootSteps, soundFootstep, -1);
    Mix_Pause(soundChannelFootSteps);

    // Collision Sound FX
    soundCollision = Mix_LoadWAV(FILENAME_HIT_SOUND);
    soundCollision->volume = 50; // reduce volume of the footsteps

    // Load Music
    music = Mix_LoadMUS(FILENAME_MUSIC);
    Mix_VolumeMusic(255);
    Mix_PlayMusic(music, -1);
}



/** Plays / Pauses the character's footsteps **/
void SoundManager::playCharacterFootsteps(void) {
    if (isMoving && playSound) {
        if (Mix_Paused(soundChannelFootSteps))
            Mix_Resume(soundChannelFootSteps);
    } else
        Mix_Pause(soundChannelFootSteps);
}

/** Plays the "clonck" sound once the player hits an object (tree, fence...) **/
void SoundManager::playCharacterHitSound(void){
    if (Mix_Playing(soundChannelCollision) != 1){
        Mix_PlayChannel(soundChannelCollision, soundCollision, 0);
    }
}
