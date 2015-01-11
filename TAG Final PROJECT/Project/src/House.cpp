#include "House.h"

#include "TextureManager.h"

House::House()
{
    //ctor
}

House::~House()
{
    //dtor
}

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void House::create(void){
    TextureManager::loadTextureDevil(TEXTURE_HOUSE_WINDOW_FILENAME, texture_house_window, false);
    TextureManager::loadTextureDevil(TEXTURE_HOUSE_DOOR_FILENAME, texture_house_door, false);
    TextureManager::loadTextureDevil(TEXTURE_HOUSE_ROOF_FILENAME, texture_house_roof, false);


    houseDL = glGenLists(1);
    glNewList(houseDL, GL_COMPILE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_house_window);

       // glPushMatrix();

                // Windows
                glBegin(GL_QUADS);
                    glNormal3f(0, 0, 1);      // Not calculated for this trivial case!
                    glTexCoord2i(0, 0); glVertex3i(-2, 0, 2);   // LB
                    glTexCoord2i(0, 1); glVertex3i(-2, 3, 2);   // LT
                    glTexCoord2i(1, 1); glVertex3i( 2, 3, 2);   // RT
                    glTexCoord2i(1, 0); glVertex3i( 2, 0, 2);   // RB
                glEnd();
                glBegin(GL_QUADS);
                    glNormal3f(0, 0, -1);      // Not calculated for this trivial case!
                    glTexCoord2i(0, 0); glVertex3i(-2, 0, -2);   // LB
                    glTexCoord2i(0, 1); glVertex3i(-2, 3, -2);   // LT
                    glTexCoord2i(1, 1); glVertex3i( 2, 3, -2);   // RT
                    glTexCoord2i(1, 0); glVertex3i( 2, 0, -2);   // RB
                glEnd();
                glBegin(GL_QUADS);
                    glNormal3f(-1, 0, 0);      // Not calculated for this trivial case!
                    glTexCoord2i(0, 0); glVertex3i(-2, 0, -2);   // LB
                    glTexCoord2i(0, 1); glVertex3i(-2, 3, -2);   // LT
                    glTexCoord2i(1, 1); glVertex3i(-2, 3,  2);   // RT
                    glTexCoord2i(1, 0); glVertex3i(-2, 0,  2);   // RB
                glEnd();

                glBindTexture(GL_TEXTURE_2D, texture_house_door);

                // Door
                glBegin(GL_QUADS);
                    glNormal3f(1, 0, 0);      // Not calculated for this trivial case!
                    glTexCoord2i(0, 0); glVertex3i( 2, 0, -2);   // LB
                    glTexCoord2i(0, 1); glVertex3i( 2, 3, -2);   // LT
                    glTexCoord2i(1, 1); glVertex3i( 2, 3,  2);   // RT
                    glTexCoord2i(1, 0); glVertex3i( 2, 0,  2);   // RB
                glEnd();

                // Roof Stuff
                glBindTexture(GL_TEXTURE_2D, texture_house_roof);
                // Roof
                glBegin(GL_QUADS);
                    glNormal3f(0.707, 0.707, 0);      // Normal at 45°
                    glTexCoord2i(0, 0); glVertex3i( 2, 3, -2);   // LB
                    glTexCoord2i(0, 1); glVertex3i( 0, 5, -2);   // LT
                    glTexCoord2i(1, 1); glVertex3i( 0, 5,  2);   // RT
                    glTexCoord2i(1, 0); glVertex3i( 2, 3,  2);   // RB
                glEnd();
                glBegin(GL_QUADS);
                    glNormal3f(-0.707, 0.707, 0);      // Normal at 45°
                    glTexCoord2i(0, 0); glVertex3i( -2, 3, -2);   // LB
                    glTexCoord2i(0, 1); glVertex3i( 0,  5, -2);   // LT
                    glTexCoord2i(1, 1); glVertex3i( 0,  5,  2);   // RT
                    glTexCoord2i(1, 0); glVertex3i( -2, 3,  2);   // RB
                glEnd();

                // Roof triangles Front / Back
                glBindTexture(GL_TEXTURE_2D, texture_house_window);
                glBegin(GL_TRIANGLES);
                    glNormal3f(0, 0, -1);      // Normal is trivial
                    glTexCoord2f(0, 0); glVertex3i( -2, 3, -2);     // LB
                    glTexCoord2f(0.5, 0.4); glVertex3i( 0,  5, -2); // CT
                    glTexCoord2f(1, 0); glVertex3i( 2,  3, -2);     // RB
                glEnd();
                glBegin(GL_TRIANGLES);
                    glNormal3f(0, 0, 1);    // Normal is trivial
                    glTexCoord2f(0, 0);     glVertex3i( -2, 3, 2);  // LB
                    glTexCoord2f(0.5, 0.4); glVertex3i( 0,  5, 2);  // CT
                    glTexCoord2f(1, 0);     glVertex3i( 2,  3, 2);  // RB
                glEnd();
            glDisable(GL_TEXTURE_2D);
       // glPopMatrix();

    glEndList();
}

void House::draw() {
    glCallList(houseDL);
}
