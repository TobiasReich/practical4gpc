#include "GroundPatch.h"
#include "TextureManager.h"

GroundPatch::GroundPatch()
{
    //ctor
}

GroundPatch::~GroundPatch()
{
    //dtor
}

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void GroundPatch::create(void){
    TextureManager::loadTextureDevil(TEXTURE_GROUND_FILENAME, texture_ground, false);

    groundPatchDL = glGenLists(1);
    glNewList(groundPatchDL, GL_COMPILE);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_ground);

        // Draw more than one quad to evate that lousy per vertex shading
        glBegin(GL_QUADS);
               // Not calculated for this trivial case!
              glNormal3f(0, 1, 0); glTexCoord2i(0, 0); glVertex3i(-1, 0, -1);   // LB
              glNormal3f(0, 1, 0); glTexCoord2i(0, 1); glVertex3i(-1, 0,  1);   // LT
              glNormal3f(0, 1, 0); glTexCoord2i(1, 1); glVertex3i( 1, 0,  1);   // RT
              glNormal3f(0, 1, 0); glTexCoord2i(1, 0); glVertex3i( 1, 0, -1);   // RB
        glEnd();

    glEndList();
}

void GroundPatch::draw() {
     glCallList(groundPatchDL);
}
