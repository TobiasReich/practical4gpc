#include "Lake.h"

#include "TextureManager.h"

Lake::Lake()
{
    //ctor
}

Lake::~Lake()
{
    //dtor
}

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void Lake::create(void){
    TextureManager::loadTextureDevil(TEXTURE_LAKE_WATER, texture_water, true);
    TextureManager::loadTextureDevil(TEXTURE_BEACH, texture_beach, false);


    lakeDL = glGenLists(1);
    glNewList(lakeDL, GL_COMPILE);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, texture_beach);
        glEnable(GL_NORMALIZE);
            // Triangle Fan to have a concentric falloff around the lake
            glBegin(GL_TRIANGLE_FAN);
                   // Not calculated for this trivial case!
                  glNormal3f(0, 1, 0); glTexCoord2f(0.5f, 0.5f); glVertex3i(0, -2.5f, 0);   // CENTER (down)
                   glTexCoord2f(0, 0); glVertex3i(-5, 0, -5);
                   glTexCoord2f(0, 1); glVertex3i(-5, 0,  5);
                   glTexCoord2f(1, 1); glVertex3i( 5, 0,  5);
                   glTexCoord2f(1, 0); glVertex3i( 5, 0, -5);
                glTexCoord2f(0, 0); glVertex3i(-5, 0, -5);
            glEnd();
        glDisable(GL_NORMALIZE);
    glEndList();
}

void Lake::draw() {
     glCallList(lakeDL);
}
