#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <IL/ilut.h>
#include <IL/il.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

using namespace std;

class SoundManager
{
    public:
        // SOUND & MUSIC -------------------------------------------------------
        int soundChannelFootSteps = 0;
        int soundChannelCollision = 1;
        Mix_Chunk *soundFootstep;
        Mix_Chunk *soundCollision;
        Mix_Music *music;

        bool playSound = true;
        bool playMusic = true;
        const char * FILENAME_HIT_SOUND = "Data/sound/hit.wav";
        const char * FILENAME_FOOTSTEP_SOUND = "Data/sound/steps.wav";
        const char * FILENAME_MUSIC = "Data/sound/music.mp3";

        bool isMoving = false;              // determines whether the player is making "moving noise"


        SoundManager();
        virtual ~SoundManager();
        void init(void);
        void initSound(void);
        void playCharacterFootsteps(void); // plays the foot steps if player is walking
        void playCharacterHitSound(void); // plays sound once the character hits an object
    protected:
    private:
};


#endif // SOUNDMANAGER_H
