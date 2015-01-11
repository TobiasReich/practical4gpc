#include "Tower.h"

#include "TextureManager.h"

Tower::Tower() {
    //ctor
}

Tower::~Tower() {
    //dtor
}

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void Tower::create(void){
    TextureManager::loadTextureDevil(TEXTURE_WALL_FILENAME, texture_wall, false);
    TextureManager::loadTextureDevil(TEXTURE_PANDA_FILENAME, texture_panda, true);

    // Trunk Object
    GLUquadricObj *flagPole = gluNewQuadric();
    gluQuadricDrawStyle(flagPole, GLU_FILL);
    gluQuadricOrientation(flagPole,  GLU_OUTSIDE);
    gluQuadricTexture(flagPole, TRUE);
    gluQuadricNormals(flagPole, GLU_SMOOTH);

    //TOWER LIST
    towerDL = glGenLists(1);
    glNewList(towerDL, GL_COMPILE);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_wall);

        glPushMatrix();
                // BACK Side (Z)
                glBegin(GL_QUADS);
                    glNormal3f(0, 0, 1);      // Not calculated for this trivial case!
                    glTexCoord2i(0, 0); glVertex3i(-1, 0, 1);   // LB
                    glTexCoord2i(0, 5); glVertex3i(-1, 5, 1);   // LT
                    glTexCoord2i(2, 5); glVertex3i( 1, 5, 1);   // RT
                    glTexCoord2i(2, 0); glVertex3i( 1, 0, 1);   // RB
                glEnd();

                // FRONT Side(-Z)
                glBegin(GL_QUADS);
                    glNormal3f(0, 0, -1);      // Not calculated for this trivial case!
                    glTexCoord2i(0, 0); glVertex3i(-1, 0, -1);   // LB
                    glTexCoord2i(0, 5); glVertex3i(-1, 5, -1);   // LT
                    glTexCoord2i(2, 5); glVertex3i( 1, 5, -1);   // RT
                    glTexCoord2i(2, 0); glVertex3i( 1, 0, -1);   // RB
                glEnd();

                // LEFT Side(-X)
                glBegin(GL_QUADS);
                    glNormal3f(-1, 0, 0);      // Not calculated for this trivial case!
                    glTexCoord2i(0, 0); glVertex3i(-1, 0, -1);   // LB
                    glTexCoord2i(0, 5); glVertex3i(-1, 5, -1);   // LT
                    glTexCoord2i(2, 5); glVertex3i(-1, 5,  1);   // RT
                    glTexCoord2i(2, 0); glVertex3i(-1, 0,  1);   // RB
                glEnd();

                // RIGHT Side(X)
                glBegin(GL_QUADS);
                    glNormal3f(1, 0, 0);      // Not calculated for this trivial case!
                    glTexCoord2i(0, 0); glVertex3i( 1, 0, -1);   // LB
                    glTexCoord2i(0, 5); glVertex3i( 1, 5, -1);   // LT
                    glTexCoord2i(2, 5); glVertex3i( 1, 5,  1);   // RT
                    glTexCoord2i(2, 0); glVertex3i( 1, 0,  1);   // RB
                glEnd();
        glPopMatrix();

        // FLAG POLE
        glPushMatrix();
            // Pole
            glTranslatef(0, 5, 0);
            glRotatef(-90, 1, 0, 0);

            glBindTexture(GL_TEXTURE_2D, texture_panda);
            gluCylinder(flagPole, 0.05, 0.05, 3, 4, 4);
        glPopMatrix();

        // Flag
        glPushMatrix();
            // Calculate light on both sides since we can see the flag from both
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
            glTranslatef(0, 6.5, 0);
            glBegin(GL_QUADS);
                glNormal3f(0, 0, 1);      // Can't use a single normals
                glTexCoord2i(0, 0); glVertex3i( 0, 0, 0);   // LB
                glTexCoord2i(0, 1); glVertex3i( 0, 1, 0);   // LT
                glTexCoord2i(1, 1); glVertex3i( -2, 1, 0);   // RT
                glTexCoord2i(1, 0); glVertex3i( -2, 0, 0);   // RB
            glEnd();
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

    glEndList();

    gluDeleteQuadric(flagPole);
}

void Tower::draw() {
     glCallList(towerDL);
}
