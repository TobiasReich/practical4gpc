#include "StarField.h"

#include "World.h"

StarField::StarField()
{
    //ctor
}

StarField::~StarField()
{
    //dtor
}

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void StarField::create(void){
 starsDL = glGenLists(1);
    glNewList(starsDL, GL_COMPILE);

        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);

        // Draw more than one quad to evate that lousy per vertex shading
        //glPushMatrix();

            // Smallest / farthest stars
            glPointSize(1);
            glColor3f(0.6f, 0.8f, 1);
            glBegin(GL_POINTS);
                for (int i=0; i< 150; ++i){
                    glVertex3f((rand() % 1000)-500, 200, (rand() % 1000)-500);
                }
            glEnd();

            // mid range stars
            glColor3f(1, 1, 1);
            glPointSize(2);
            glBegin(GL_POINTS);
                for (int i=0; i< 50; ++i){
                    glVertex3f((rand() % 1000)-500, 150, (rand() % 1000)-500);
                }
            glEnd();

            // big / near stars
            World::resetGLColor();
            glPointSize(3);
            glBegin(GL_POINTS);
                for (int i=0; i< 25; ++i){
                    glVertex3f((rand() % 1000)-500, 90, (rand() % 1000)-500);
                }
            glEnd();
        //glPopMatrix();

        glEnable(GL_LIGHTING);
    glEndList();
}

void StarField::draw() {
     glCallList(starsDL);
}
