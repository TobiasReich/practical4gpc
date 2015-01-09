#include "World.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>


#include "GameObject.h"     // Drawable GameObject
#include "Player.h"         // Player related class
#include "ASEParser.h"      // Importer for ASE Files


#define PI 3.14159265

using namespace std;

GameObject go;              // Test Object

World::World() {
    //ctor
}

World::~World() {
    //dtor
}

void World::loadWorld(void){
    // Fog for better atmosphere
    // @TODO Put that in a function to switch it on/off
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, FogColor);    // set the fog color
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 15.f);
    glFogf(GL_FOG_END, 450.f);


    // Create Parser Object and parse the "filename"
    ASEParser parser(filename);
    parser.readObject(vertices, normals, faces);

    // Create  Display Lists
    createImportedObjectDL();   // Imported Object
    createHouseDL();            // create the house DL
    createTowerDL();            // Tower
    createMoonDL();             // The Moon
    createGroundPatchDL();      // Ground Patch DL
    createTreeDLs();
    createStarDL();             // Creates a DL for stars in the sky
    createLakeDL();             // Creates a DL for the lake
    initTreePositions();
    createSkylineDL();          // Create the DL for the Skyline
}


/** Creates a display-list for the 2x2 ground patch **/
void World::createGroundPatchDL(){
    groundPatchDL = glGenLists(1);
    glNewList(groundPatchDL, GL_COMPILE);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_ground);

        // Draw more than one quad to evate that lousy per vertex shading
        //glPushMatrix();

            glBegin(GL_QUADS);
                   // Not calculated for this trivial case!
                  glNormal3f(0, 1, 0); glTexCoord2i(0, 0); glVertex3i(-1, 0, -1);   // LB
                  glNormal3f(0, 1, 0); glTexCoord2i(0, 1); glVertex3i(-1, 0,  1);   // LT
                  glNormal3f(0, 1, 0); glTexCoord2i(1, 1); glVertex3i( 1, 0,  1);   // RT
                  glNormal3f(0, 1, 0); glTexCoord2i(1, 0); glVertex3i( 1, 0, -1);   // RB
            glEnd();
        //glPopMatrix();

    glEndList();
}


/** Creates a display-list for stars **/
void World::createStarDL(){
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
            resetGLColor();
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

/** This creates the GL-Display-List in order to render the imported Object.
@see drawImportedObject(); **/
void World::createImportedObjectDL(){
    importObjectDL = glGenLists(1);
    glNewList(importObjectDL, GL_COMPILE);

        glDisable(GL_BLEND);

        // These objects appear far to bright. This reduces the diffuse color greatly
        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glColor3f(0.02f, 0.02, 0.02f);


        glPushMatrix();
            glScalef(0.002, 0.002, 0.002);    // These objetcs are huge!

            // Rotate since axis are different
            // X / Y like screen cordinates for better understanding
            // since 2D is always X/Y, Z is depth
            // This leads to: X = left-right, Y = up-down, Z = back-forth
            glRotatef(-90, 1, 0, 0);

            // For every surfaces we have -> Draw it with corresponding normal
            // std::size_t instead of int in order to not compare (un)signed integers.
            // Used since size() is expressed in std::size_t (like sizeof...)
            for (std::size_t i=0; i<faces.size(); ++i){
                    ASEParser::Face face = faces[i];

                    // Vertex points
                    ASEParser::Vertex vA = vertices[face.a];
                    ASEParser::Vertex vB = vertices[face.b];
                    ASEParser::Vertex vC = vertices[face.c];

                    ASEParser::Normal nA = normals[face.a];
                    ASEParser::Normal nB = normals[face.b];
                    ASEParser::Normal nC = normals[face.c];

                    // Draw the object
                    // Using per Vertex Normals for smoother interpolation
                glBegin(GL_TRIANGLES);
                    glNormal3f(nA.x, nA.y, nA.z); glVertex3i(vA.x, vA.y, vA.z);
                    glNormal3f(nB.x, nB.y, nB.z); glVertex3i(vB.x, vB.y, vB.z);
                    glNormal3f(nC.x, nC.y, nC.z); glVertex3i(vC.x, vC.y, vC.z);
                glEnd();
            }
        glPopMatrix();

        // Reset the original diffuse material value
        resetGLColor();
    glEndList();
}


/** Creates the house **/
void World::createHouseDL(void) {
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

/** Creates the Display-List for the tower (so we can reuse it several times)**/
void World::createTowerDL(void){

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

/** Creates the Display List for the moon**/
void World::createMoonDL(void){
    moonDL = glGenLists(1);
    glNewList(moonDL , GL_COMPILE);

        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, texture_moon);

       // glPushMatrix();
                glBegin(GL_QUADS);
                    glTexCoord2i(0, 0); glVertex3i(-55, -55, 0);   // LB
                    glTexCoord2i(0, 1); glVertex3i(-55,  55, 0);   // LT
                    glTexCoord2i(1, 1); glVertex3i( 55,  55, 0);   // RT
                    glTexCoord2i(1, 0); glVertex3i( 55, -55, 0);   // RB
                glEnd();
       // glPopMatrix();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    glEndList();
}

/** Creates the Sky Boy for the World (trees, stars...) **/
void World::createSkylineDL(void){

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
   // glPopMatrix();

       // glEnable(GL_LIGHTING);
    glEndList();

    // -- SKYLINE --

    skylineDL = glGenLists(1);
    glNewList(skylineDL, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, texture_skyline);

    //glPushMatrix();
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
    //glPopMatrix();

    glEnable(GL_LIGHTING);
    glEndList();
}

/** Creates a simple Tree made of a cylinder and a sphere as top **/
void World::createTreeDLs(){
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

    // SMALL TREES
    treeSmallDL = glGenLists(1);
    glNewList(treeSmallDL, GL_COMPILE);

        glEnable(GL_TEXTURE_2D);

       // glPushMatrix();
            // Trunk
            glTranslatef(0, 2.5, 0);
            glRotatef(90, 1, 0, 0);

            glBindTexture(GL_TEXTURE_2D, texture_trunk);
            gluCylinder(trunk, 0.05, 0.15, 2.5, 6, 6);

            //Treetop
            glBindTexture(GL_TEXTURE_2D, texture_plant);

            glTranslatef(0, 0, -0.6);
            gluSphere(sphere, 0.8, 6, 6);

        //glPopMatrix();

    glEndList();


    // MEDIUM TREES
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

    // LARGE TREES
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

    // Delete Memory usage
    gluDeleteQuadric(sphere);
    gluDeleteQuadric(trunk);
}


/** Initializes the tree positions **/
void World::initTreePositions(){
    for (int i=0; i< 15; ++i){
        Position2D pos = {(rand() % 80)-40, (rand() % 40) - 20};
        treeSmallPositions.push_back(pos);
    }
    for (int i=0; i< 10; ++i){
        Position2D pos = {(rand() % 80)-40, (rand() % 40) - 20};
        treeNormalPositions.push_back(pos);
    }
    for (int i=0; i< 5; ++i){
        Position2D pos = {(rand() % 80)-40, (rand() % 40) - 20};
        treeBigPositions.push_back(pos);
    }
}

/** Creates the Display List for the lake**/
void World::createLakeDL(void){
    lakeDL = glGenLists(1);
    glNewList(lakeDL, GL_COMPILE);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, texture_beach);
        glEnable(GL_NORMALIZE);
       // glPushMatrix();
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
      //  glPopMatrix();
        glDisable(GL_NORMALIZE);
    glEndList();
}

// --------------------------


/** Convenience method for creating the "level" **/
void World::drawWorld(void){
    setMoonLighting();      // sets the "fixed" light of the scene

    moonRot += 0.05f;        // Rorate the moon (& stars) even if we don't see it
    drawStars();            // Stars in the sky (3 types)
    if (moonLight){
        drawMoon();         // the moon (if active)
    }
    drawSkyline();          // Draws the Skybox

    drawGround();           // The ground
    drawTowers();           // Towers in the corners
    drawHouse();            // the estate in the center

    drawLake();             // Draws the (transparent) lake in the world

    drawImportedObject();   // the imported Object (final = fountain)

    drawFence();            // At the end (after towers & ground) since uses blending!!!
    drawTrees();            // After the fence since the grass is transparent

    go.draw();              // Test for drawing a GO by itself.
}

/** Draws the iported AES object (Teapot, Box...)
@TODO: Place it at a certain position etc. **/
void World::drawImportedObject(void){
    glPushMatrix();
        glTranslatef(fountainPos.lat, 0, fountainPos.lon);
        glCallList(importObjectDL);
    glPopMatrix();

}

/** Draws the house in the level**/
void World::drawHouse(void){
    glPushMatrix();
        glTranslatef(housePosition.lat, 0, housePosition.lon);
        glCallList(houseDL);
    glPopMatrix();
}

/** Draws the tower (display list) **/
void World::drawTowers(){
    // Move the towers 1 "meter" inside the fence so they look better e.g. not 50 but only 49 from center
    glPushMatrix();
        glTranslatef(49, 0, 24);
        glCallList(towerDL);

        glTranslatef(0, 0, -48);
        glCallList(towerDL);

        glTranslatef(-98, 0, 0);
        glCallList(towerDL);

        glTranslatef(0, 0, 48);
        glCallList(towerDL);
    glPopMatrix();
 }

 /** Draws the house in the level**/
void World::drawMoon(void){

    float altitude = sin(moonRot * PI / 180.0);
    glPushMatrix();
        glRotatef(moonRot, 0, 1, 0);
        glTranslatef(0, 70 * altitude + 40, 250);
        glCallList(moonDL);
    glPopMatrix();
}

void World::drawStars(void){
    glPushMatrix();
        glRotatef(moonRot * 0.1, 0, 1, 0);
        glCallList(starsDL);
    glPopMatrix();
}


/** Draws the ground, consisting of several identical patches (so we have a finer grid) **/
void World::drawGround(void){
    glPushMatrix();
        glTranslatef(-100, 0, -100);
        for (int i=0; i<100; ++i){
            glTranslatef(2, 0, 0);
            glPushMatrix();
                for (int j=0; j<100; ++j){
                        glTranslatef(0, 0, 2);
                        if ((i == 52 || i == 53 || i == 54 || i == 55 || i == 56)
                            && (j == 52 || j == 53 || j == 54 || j == 55 || j == 56 )){
                            // Do nothing -> leaves empty space
                        } else {
                            glCallList(groundPatchDL); //draws the ground patch
                        }
                }
            glPopMatrix();
        }
    glPopMatrix();
}


/** No display list for the fence since we just draw a quad
and we have different side lengths for the fence (park is 100 x 50)**/
void World::drawFence(){
    // Ground
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, texture_fence);

    glPushMatrix();
        glBegin(GL_QUADS);
            glNormal3f(0, 0,-1);      // Not calculated for this trivial case!
            glTexCoord2i(0,  0); glVertex3i(-50, 0, 25);   // LB
            glTexCoord2i(0,  1); glVertex3i(-50, 2, 25);   // LT
            glTexCoord2i(50, 1); glVertex3i( 50, 2, 25);   // RT
            glTexCoord2i(50, 0); glVertex3i( 50, 0, 25);   // RB
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);      // Not calculated for this trivial case!
            glTexCoord2i(0,  0); glVertex3i(-50, 0, -25);   // LB
            glTexCoord2i(0,  1); glVertex3i(-50, 2, -25);   // LT
            glTexCoord2i(50, 1); glVertex3i( 50, 2, -25);   // RT
            glTexCoord2i(50, 0); glVertex3i( 50, 0, -25);   // RB
        glEnd();
    glPopMatrix();

        glPushMatrix();
        glBegin(GL_QUADS);
            glNormal3f(-1, 0, 0);      // Not calculated for this trivial case!
            glTexCoord2i(0,  0); glVertex3i( 50, 0, -25);   // LB
            glTexCoord2i(0,  1); glVertex3i( 50, 2, -25);   // LT
            glTexCoord2i(50, 1); glVertex3i( 50, 2, 25);   // RT
            glTexCoord2i(50, 0); glVertex3i( 50, 0, 25);   // RB
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(1, 0, 0);      // Not calculated for this trivial case!
            glTexCoord2i(0,  0); glVertex3i(-50, 0, -25);   // LB
            glTexCoord2i(0,  1); glVertex3i(-50, 2, -25);   // LT
            glTexCoord2i(50, 1); glVertex3i(-50, 2,  25);   // RT
            glTexCoord2i(50, 0); glVertex3i(-50, 0,  25);   // RB
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

/** Draws all 3 types of trees **/
void World::drawTrees(){
    for (std::size_t i = 0; i< treeSmallPositions.size(); ++i){
        Position2D pos = treeSmallPositions[i];
        glPushMatrix();
            glTranslatef(pos.lat, 0, pos.lon);
            glCallList(treeSmallDL);
        glPopMatrix();
    }
    for (std::size_t i = 0; i< treeNormalPositions.size(); ++i){
        Position2D pos = treeNormalPositions[i];
        glPushMatrix();
            glTranslatef(pos.lat, 0, pos.lon);
            glCallList(treeNormalDL);
        glPopMatrix();
    }
    for (std::size_t i = 0; i< treeBigPositions.size(); ++i){
        Position2D pos = treeBigPositions[i];
        glPushMatrix();
            glTranslatef(pos.lat, 0, pos.lon);
            glCallList(treeBigDL);
        glPopMatrix();
    }
}

/** Draws the lake (Display List) in the world **/
void World::drawLake(void){
     glPushMatrix();
        glTranslatef(lakePos.lat, 0, lakePos.lon);
        glCallList(lakeDL);

        // Draw a "rotating water" on top of the hole
           glTranslatef(0, -0.3f, 0); // Water on top (but still a bit below normal level)
           glRotatef(moonRot, 0,1,0);
            glBindTexture(GL_TEXTURE_2D, texture_water);
            glBegin(GL_QUADS);
                  glNormal3f(0, 1, 0); glTexCoord2f(0, 0); glVertex3i(-5, 0, -5);   // LB
                  glNormal3f(0, 1, 0); glTexCoord2f(0, 1); glVertex3i(-5, 0,  5);   // LT
                  glNormal3f(0, 1, 0); glTexCoord2f(1, 1); glVertex3i( 5, 0,  5);   // RT
                  glNormal3f(0, 1, 0); glTexCoord2f(1, 0); glVertex3i( 5, 0, -5);   // RB
            glEnd();
    glPopMatrix();
}

/** Draws the Skybox (Clouds) and Skyline **/
void World::drawSkyline(void){
    glPushMatrix();
        glRotatef(- moonRot * 0.2, 0, 1, 0);
        glCallList(cloudsDL);
    glPopMatrix();
    glCallList(skylineDL);
}

/** This sets the moon light.
Unlike setPlayerLight() this is not moving with the player.
(i.e. is translated according to the Matrix-Stack)
BUT it is moving with the drawn moon (which has its own rotation)
@see World::drawMoon() **/
void World::setMoonLighting(void){
    // Move the light according to the moon position
    float moonLat = sin(moonRot * PI / 180.0);
    float moonLon = cos(moonRot * PI / 180.0);
    // Light is slightly inside of the "world" so we can see the spot a bit better
    GLfloat positionmoonLight[] = {80.0f * moonLat, 20.0f, 40.0f * moonLon, 1.0f};

    // Dim the light when the moon goes down
    float brightness = std::max(moonLat, 0.0f);
    GLfloat light0SceneDiffuse[] = {90.0f * brightness, 100.0f * brightness, 140.0f * brightness, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0SceneDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, positionmoonLight);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
    // glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 2.0);
    if (moonLight)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
}

// Collision detection is done here (inside the world) ---------


/** Checks whether the player "is near" to a target object
    @return  bool true if object is near (HIT) **/
bool World::isNearTo(float newX, float newZ, World::Position2D &target){
    // if any of x or z position is not close the player can't hit the object
    if (newX + Player::PLAYER_SIZE < target.lat || newX - Player::PLAYER_SIZE > target.lat)
        return false;
    if (newZ + Player::PLAYER_SIZE < target.lon || newZ - Player::PLAYER_SIZE > target.lon)
        return false;

    return true;   // No collision -> move!
}

/** Checks whether the player "is near" to the house
    @return  bool true if player is near the house (HIT) **/
bool World::isNearToHouse(float newX, float newZ){
    // if any of x or z position is not close the player can't hit the object
    if (newX + Player::PLAYER_SIZE + 2 < housePosition.lat || newX - Player::PLAYER_SIZE - 2 > housePosition.lat)
        return false;
    if (newZ + Player::PLAYER_SIZE + 2 < housePosition.lon || newZ - Player::PLAYER_SIZE - 2 > housePosition.lon)
        return false;

    return true;   // No collision -> move!
}



/** This function checks whether the movement of the player leads to a collision.
It has to be called before the movement is executed and returns a boolean
@return true if Collision (player HITS OBJECT)
        false if player may move (FREE)! **/
bool World::checkCollision(float newX, float newZ){
    if (newX + Player::PLAYER_SIZE > 50 || newX - Player::PLAYER_SIZE < -50){
        return true;
    }
    if (newZ + Player::PLAYER_SIZE > 25 || newZ - Player::PLAYER_SIZE < -25){
        return true;
    }

    for (std::size_t i = 0; i< treeSmallPositions.size(); ++i){
        if (isNearTo(newX, newZ, treeSmallPositions[i])){
            return true;
        }
    }

    for (std::size_t i = 0; i< treeNormalPositions.size(); ++i){
        if (isNearTo(newX, newZ, treeNormalPositions[i])){
            return true;
        }
    }

    for (std::size_t i = 0; i< treeBigPositions.size(); ++i){
        if (isNearTo(newX, newZ, treeBigPositions[i])){
            return true;
        }
    }

    // Check if we hit the house
    if (isNearToHouse(newX, newZ)){
            return true;
    }
    return false;   // No collision -> move!
}

/** This updates the camera movement**/
void World::updateCameras(void){
    camRotation += 0.2f;
    // Okay, this can be made better but after all, it is fast!
    camDest.lat = 15 * sin(camRotation * PI / 180.0);
    camDest.lon = 15 * cos(camRotation * PI / 180.0);
}

/** Resets the glColor to a fixed value
(so we may call this if we want to "reset" to a global value **/
void World::resetGLColor(void){
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glColor3f(1, 0.8f, 0.5f);
}
