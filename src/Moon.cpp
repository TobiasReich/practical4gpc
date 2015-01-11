#include "Moon.h"

#include <GL/freeglut.h>
#include <GL/gl.h>

Moon::Moon()
{
    //ctor
}

Moon::~Moon()
{
    //dtor
}

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void Moon::create(void){
    TextureManager::loadTextureDevil(TEXTURE_MOON_FILENAME, texture_moon, true);

    moonDL = glGenLists(1);
    glNewList(moonDL , GL_COMPILE);

        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, texture_moon);

        glBegin(GL_QUADS);
            glTexCoord2i(0, 0); glVertex3i(-65, -65, 0);   // LB
            glTexCoord2i(0, 1); glVertex3i(-65,  65, 0);   // LT
            glTexCoord2i(1, 1); glVertex3i( 65,  65, 0);   // RT
            glTexCoord2i(1, 0); glVertex3i( 65, -65, 0);   // RB
        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    glEndList();
}

void Moon::draw() {
     glCallList(moonDL);
}



