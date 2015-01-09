#ifndef TOWER_H
#define TOWER_H


#include <GL/freeglut.h>
#include <GL/gl.h>

#include "GameObject.h"


class Tower : public GameObject
{
    public:
        const char * TEXTURE_WALL_FILENAME      = "Data/gfx/brickwall.png";
        const char * TEXTURE_PANDA_FILENAME      = "Data/gfx/panda.png";

        GLuint texture_wall;                // texture for the ground
        GLuint towerDL;                     // The Display-List for the towers
        GLuint texture_panda;               // texture for the tree top

        Tower();
        virtual ~Tower();
        void create(void) override;
        void draw(void) override;
    protected:
    private:

};

#endif // TOWER_H
