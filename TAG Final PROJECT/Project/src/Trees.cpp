#include "Trees.h"
#include "TextureManager.h"

Trees::Trees(){}

Trees::~Trees(){}


/** Creates the Display-List for the tower (so we can reuse it several times)**/
void Trees::create(void){
    TextureManager::loadTextureDevil(TEXTURE_GRASS_FILENAME, texture_grass, true);
    TextureManager::loadTextureDevil(TEXTURE_PLANT_FILENAME, texture_plant, false);
    TextureManager::loadTextureDevil(TEXTURE_TRUNK_FILENAME, texture_trunk, false);

    // Trunk Object
    GLUquadricObj *trunk = gluNewQuadric();
    gluQuadricDrawStyle(trunk, GLU_FILL);
    gluQuadricOrientation(trunk,  GLU_OUTSIDE);
    gluQuadricTexture(trunk, TRUE);
    gluQuadricNormals(trunk, GLU_SMOOTH);

    // Tree Top
    GLUquadricObj *sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

        createSmallTree(trunk, sphere);
        createMediumTree(trunk, sphere);
        createBigTree(trunk, sphere);

    // Delete Memory usage
    gluDeleteQuadric(sphere);
    gluDeleteQuadric(trunk);
}


/** Creates the Display-List for the small tree)**/
void Trees::createSmallTree(GLUquadricObj *trunk, GLUquadricObj *sphere){
    treeSmallDL = glGenLists(1);
    glNewList(treeSmallDL, GL_COMPILE);

        glEnable(GL_TEXTURE_2D);
            // Trunk
            glTranslatef(0, 2.5, 0);
            glRotatef(90, 1, 0, 0);

            glBindTexture(GL_TEXTURE_2D, texture_trunk);
            gluCylinder(trunk, 0.05, 0.15, 2.5, 6, 6);

            //Treetop
            glBindTexture(GL_TEXTURE_2D, texture_plant);

            glTranslatef(0, 0, -0.6);
            gluSphere(sphere, 0.8, 6, 6);

    glEndList();
}

/** Creates the Display-List for the medium tree **/
void Trees::createMediumTree(GLUquadricObj *trunk, GLUquadricObj *sphere){
    treeNormalDL = glGenLists(1);
    glNewList(treeNormalDL, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
            // Trunk
            glTranslatef(0, 3.5, 0);
            glRotatef(90, 1, 0, 0);

            glBindTexture(GL_TEXTURE_2D, texture_trunk);
            gluCylinder(trunk, 0.1, 0.2, 3.5, 8, 8);

            //Treetop
            glBindTexture(GL_TEXTURE_2D, texture_plant);

            glTranslatef(0, 0, -1);
            gluSphere(sphere, 1.0, 6, 6);

            glTranslatef(0, 0, -1);
            gluSphere(sphere, 0.5f, 5, 5);

            glTranslatef(0, 0, 1);

            for (int i=0; i<8; ++i){
                glPushMatrix();
                    glRotatef(45*i, 0, 0, 1);
                    glTranslatef(0, 1.1f, 0.8f);
                    gluSphere(sphere, 0.7, 7, 7);
                glPopMatrix();
            }

            for (int i=0; i<10; ++i){
                glPushMatrix();
                    glRotatef(36*i, 0, 0, 1);
                    glTranslatef(0, 1, 0);
                    gluSphere(sphere, 0.45, 5, 5);
                glPopMatrix();
            }
        glPopMatrix();

        // Weird grass around the trunks -> not sure we want that.
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture_grass);
            glEnable(GL_BLEND);

            glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);      // Not calculated for this trivial case!
                glTexCoord2i(0, 0);  glVertex3f(-0.9f,-0.2f, 0);   // LB
                glTexCoord2i(0, 1);  glVertex3f(-0.9f, 0.7f, 0);   // LT
                glTexCoord2i(1, 1);  glVertex3f( 0.9f, 0.7f, 0);   // RT
                glTexCoord2i(1, 0);  glVertex3f( 0.9f,-0.2f, 0);   // RB
            glEnd();

            glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);      // Not calculated for this trivial case!
                glTexCoord2i(0, 0);  glVertex3f( 0,-0.2f, -0.9f);   // LB
                glTexCoord2i(0, 1);  glVertex3f( 0, 0.7f, -0.9f);   // LT
                glTexCoord2i(1, 1);  glVertex3f( 0, 0.7f,  0.9f);   // RT
                glTexCoord2i(1, 0);  glVertex3f( 0,-0.2f,  0.9f);   // RB
            glEnd();

            glDisable(GL_BLEND);
        glPopMatrix();
    glEndList();
}

/** Creates the Display-List for the big tree **/
void Trees::createBigTree(GLUquadricObj *trunk, GLUquadricObj *sphere){
    treeBigDL = glGenLists(1);
    glNewList(treeBigDL, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
            // Trunk

            glTranslatef(0, 7, 0);
            glRotatef(90, 1, 0, 0);

            glBindTexture(GL_TEXTURE_2D, texture_trunk);
            gluCylinder(trunk, 0.3, 0.5, 7, 8, 8);
                // Branches
                glPushMatrix();
                    glTranslatef(0, 0, 4.5);
                    glRotatef(150, 0, 1, 0);
                    gluCylinder(trunk, 0.4, 0.2, 1.5, 8, 8);
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(0, 2, 1);
                    glRotatef(40, 1, 0, 0);
                    gluCylinder(trunk, 0.1, 0.2, 3, 8, 8);
                glPopMatrix();
            //Treetop
            glBindTexture(GL_TEXTURE_2D, texture_plant);

            glTranslatef(0, 0, -1);
            gluSphere(sphere, 3.0, 6, 6);

            // low
            for (int i=0; i<10; ++i){
                glPushMatrix();
                    glRotatef(36*i, 0, 0, 1);
                    glTranslatef(0, 2.5f, 1.5f);
                    gluSphere(sphere, 1.5, 7, 7);
                glPopMatrix();
            }
            // mid
            for (int i=0; i<12; ++i){
                glPushMatrix();
                    glRotatef(30*i, 0, 0, 1);
                    glTranslatef(0, 2, 0);
                    gluSphere(sphere, 1, 5, 5);
                glPopMatrix();
            }
            // top
            for (int i=0; i<12; ++i){
                glPushMatrix();
                    glRotatef(30*i, 0, 0, 1);
                    glTranslatef(0, 3, 3);
                    gluSphere(sphere, 1, 5, 5);
                glPopMatrix();
            }

        glPopMatrix();

        // Weird grass around the trunks -> not sure we want that.
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture_grass);
            glEnable(GL_BLEND);

            glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);      // Not calculated for this trivial case!
                glTexCoord2i(0, 0);  glVertex3f(-1,-0.2f, 0);   // LB
                glTexCoord2i(0, 1);  glVertex3f(-1, 1, 0);   // LT
                glTexCoord2i(1, 1);  glVertex3f( 1, 1, 0);   // RT
                glTexCoord2i(1, 0);  glVertex3f( 1,-0.2f, 0);   // RB
            glEnd();

            glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);      // Not calculated for this trivial case!
                glTexCoord2i(0, 0);  glVertex3f( 0,-0.2f, -1);   // LB
                glTexCoord2i(0, 1);  glVertex3f( 0, 1, -1);   // LT
                glTexCoord2i(1, 1);  glVertex3f( 0, 1,  1);   // RT
                glTexCoord2i(1, 0);  glVertex3f( 0,-0.2f,  1);   // RB
            glEnd();

            glDisable(GL_BLEND);
        glPopMatrix();
    glEndList();
}

/** Draws a small tree **/
void Trees::drawSmallTree() {
     glCallList(treeSmallDL);
}

/** Draws a medium tree **/
void Trees::drawMediumTree() {
     glCallList(treeNormalDL);
}

/** Draws a big tree **/
void Trees::drawBigTree() {
     glCallList(treeBigDL);
}

