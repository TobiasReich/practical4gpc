#include "Skyline.h"

#include <GL/freeglut.h>

#include "TextureManager.h"

Skyline::Skyline()
{
    //ctor
}

Skyline::~Skyline()
{
    //dtor
}

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void Skyline::create(void){
    TextureManager::loadTextureDevil(TEXTURE_SKYLINE_FILENAME, texture_skyline, true);
    TextureManager::loadTextureDevil(TEXTURE_CLOUDS_FILENAME, texture_clouds, false);

    cloudsDL= glGenLists(1);
    glNewList(cloudsDL, GL_COMPILE);

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, texture_clouds);

   // CLOUDS
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i(-150, 00, 150);
            glTexCoord2f(0, 1); glVertex3i(-150, 50, 150);
            glTexCoord2f(2, 1); glVertex3i( 150, 50, 150);
            glTexCoord2f(2, 0); glVertex3i( 150, 0, 150);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i(-150, 0, -150);
            glTexCoord2f(0, 1); glVertex3i(-150, 50, -150);
            glTexCoord2f(2, 1); glVertex3i( 150, 50, -150);
            glTexCoord2f(2, 0); glVertex3i( 150, 0, -150);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i(-150, 0,-150);
            glTexCoord2f(0, 1); glVertex3i(-150, 50, -150);
            glTexCoord2f(2, 1); glVertex3i(-150, 50,  150);
            glTexCoord2f(2, 0); glVertex3i(-150, 0, 150);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i( 150, 0,-150);
            glTexCoord2f(0, 1); glVertex3i( 150, 50, -150);
            glTexCoord2f(2, 1); glVertex3i( 150, 50,  150);
            glTexCoord2f(2, 0); glVertex3i( 150, 0, 150);
        glEnd();
        // TOP
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i(-150, 50, 150);
            glTexCoord2f(0, 2); glVertex3i(-150, 50, -150);
            glTexCoord2f(2, 2); glVertex3i( 150, 50, -150);
            glTexCoord2f(2, 0); glVertex3i( 150, 50, 150);
        glEnd();
    glEndList();

    // -- SKYLINE --

    skylineDL = glGenLists(1);
    glNewList(skylineDL, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, texture_skyline);

        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i(-100, 0, 100);
            glTexCoord2f(0, 1); glVertex3i(-100, 40, 100);
            glTexCoord2f(3, 1); glVertex3i( 100, 40, 100);
            glTexCoord2f(3, 0); glVertex3i( 100, 0, 100);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i(-100, 0, -100);
            glTexCoord2f(0, 1); glVertex3i(-100, 40, -100);
            glTexCoord2f(3, 1); glVertex3i( 100, 40, -100);
            glTexCoord2f(3, 0); glVertex3i( 100, 0, -100);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i(-100, 0,-100);
            glTexCoord2f(0, 1); glVertex3i(-100, 40, -100);
            glTexCoord2f(3, 1); glVertex3i(-100, 40,  100);
            glTexCoord2f(3, 0); glVertex3i(-100, 0, 100);
        glEnd();
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3i( 100, 0,-100);
            glTexCoord2f(0, 1); glVertex3i( 100, 40, -100);
            glTexCoord2f(3, 1); glVertex3i( 100, 40,  100);
            glTexCoord2f(3, 0); glVertex3i( 100, 0, 100);
        glEnd();

    glEnable(GL_LIGHTING);
    glEndList();
}

void Skyline::draw() {
    glCallList(skylineDL);
}

void Skyline::drawClouds() {
    glCallList(cloudsDL);
}
