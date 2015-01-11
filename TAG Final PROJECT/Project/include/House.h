#ifndef HOUSE_H
#define HOUSE_H

#include "GameObject.h"

#include <GL/freeglut.h>


class House : public GameObject
{
    public:
        GLuint texture_house_window;        // texture for the house wall with window
        GLuint texture_house_door;          // texture for the house wall with door
        GLuint texture_house_roof;          // texture for the house roof

        const char * TEXTURE_HOUSE_WINDOW_FILENAME  = "Data/gfx/house_window.png";
        const char * TEXTURE_HOUSE_DOOR_FILENAME = "Data/gfx/house_door.png";
        const char * TEXTURE_HOUSE_ROOF_FILENAME = "Data/gfx/house_roof.png";

        GLuint houseDL;                     // The Display-List for the house patches

        House();
        virtual ~House();

        void create(void) override;
        void draw(void) override;
    protected:
    private:
};

#endif // HOUSE_H
