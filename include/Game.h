#ifndef GAME_H
#define GAME_H

#include "Player.h"         // Player related class
#include "SoundManager.h"   // Class responsible for sound & music
#include "World.h"          // World Class

class Game
{
    public:
        // Constants for defining the cameras
        static const int CAM_PLAYER = 0;
        static const int CAM_TOWER_1 = 1;
        static const int CAM_TOWER_2 = 2;
        static const int CAM_TOWER_3 = 3;
        static const int CAM_TOWER_4 = 4;
        static const int CAM_HOUSE = 5;

        //@todo THIS COULD BE DYNAMIC!!!!
        static const int MMAP_WIDTH = 400;
        static const int MMAP_HEIGHT = 200;
        static const int MMAP_MARGIN = 50;

        Game();
        virtual ~Game();

        void initGame(void);    // Initializes game values and fields
        void startGame(void);   // starts the game (loop etc.)
    protected:
    private:
};

#endif // GAME_H
