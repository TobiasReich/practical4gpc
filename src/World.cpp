#include "World.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>


#include "GameObject.h"     // Drawable GameObject
#include "GroundPatch.h"    // Drawable GameObject (Ground)
#include "StarField.h"      // Drawable GameObject (Stars)
#include "Tower.h"          // Drawable GameObject (Tower)
#include "House.h"          // Drawable GameObject (House)
#include "Moon.h"           // Drawable GameObject (Moon)
#include "Skyline.h"        // Drawable GameObject (Skyline)
#include "Lake.h"           // Drawable GameObject (Lake)
#include "Trees.h"          // Drawable GameObjects (Trees - small, medium, big)
#include "Fountain.h"       // Imported Fountain from ASE File

#include "Player.h"         // Player related class
#include "ASEParser.h"      // Importer for ASE Files


#define PI 3.14159265

using namespace std;

// GameObjects for drawing etc.
Moon moon;
Tower tower;
House house;
Lake lake;
Skyline skyline;
GroundPatch groundPatch;
StarField starField;
Trees trees;
Fountain fountain;

World::World() {}

World::~World() {}

void World::loadWorld(void){
    // Fog for better atmosphere
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, FogColor);    // set the fog color
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 15.f);
    glFogf(GL_FOG_END, 450.f);


    // Create GameObjects
    fountain.create();
    tower.create();
    moon.create();
    house.create();
    lake.create();
    skyline.create();
    groundPatch.create();
    starField.create();

    trees.create();
    initTreePositions();
}


/** Initializes the tree positions **/
void World::initTreePositions(){
    for (int i=0; i< 20; ++i){
        Position2D pos = {(rand() % 80)-40, (rand() % 40) - 20};
        treeSmallPositions.push_back(pos);
    }
    for (int i=0; i< 10; ++i){
        Position2D pos = {(rand() % 80)-40, (rand() % 40) - 20};
        treeNormalPositions.push_back(pos);
    }
    for (int i=0; i< 8; ++i){
        Position2D pos = {(rand() % 80)-40, (rand() % 40) - 20};
        treeBigPositions.push_back(pos);
    }
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

    drawFountain();   // the imported Object (final = fountain)

    drawFence();            // At the end (after towers & ground) since uses blending!!!
    drawTrees();            // After the fence since the grass is transparent
}

/** Draws the iported AES object (Teapot, Box...) **/
void World::drawFountain(void){
    glPushMatrix();
        glTranslatef(fountainPos.lat, 0, fountainPos.lon);
        fountain.draw();
    glPopMatrix();
}

/** Draws the house in the level**/
void World::drawHouse(void){
    glPushMatrix();
        glTranslatef(housePosition.lat, 0, housePosition.lon);
        house.draw();
    glPopMatrix();
}

/** Draws the tower (display list) **/
void World::drawTowers(){
    // Move the towers 1 "meter" inside the fence so they look better e.g. not 50 but only 49 from center
    glPushMatrix();
        glTranslatef(49, 0, 24);
        tower.draw();

        glTranslatef(0, 0, -48);
        tower.draw();

        glTranslatef(-98, 0, 0);
        tower.draw();

        glTranslatef(0, 0, 48);
        tower.draw();
    glPopMatrix();
 }

 /** Draws the house in the level**/
void World::drawMoon(void){
    float altitude = sin(moonRot * PI / 180.0);
    glPushMatrix();
        glRotatef(moonRot, 0, 1, 0);
        glTranslatef(0, 70 * altitude + 40, 250);
        moon.draw();
    glPopMatrix();
}

/** Draws the stars. They are moving in the direction
of the moon (unlike the clouds) - just slower **/
void World::drawStars(void){
    glPushMatrix();
        glRotatef(moonRot * 0.1, 0, 1, 0);
        starField.draw();
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
                            groundPatch.draw();
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
           trees.drawSmallTree();
        glPopMatrix();
    }
    for (std::size_t i = 0; i< treeNormalPositions.size(); ++i){
        Position2D pos = treeNormalPositions[i];
        glPushMatrix();
            glTranslatef(pos.lat, 0, pos.lon);
            trees.drawMediumTree();
        glPopMatrix();
    }
    for (std::size_t i = 0; i< treeBigPositions.size(); ++i){
        Position2D pos = treeBigPositions[i];
        glPushMatrix();
            glTranslatef(pos.lat, 0, pos.lon);
            trees.drawBigTree();
        glPopMatrix();
    }
}

/** Draws the lake (Display List) in the world **/
void World::drawLake(void){
     glPushMatrix();
        glTranslatef(lakePos.lat, 0, lakePos.lon);
        lake.draw();

        // Draw a "rotating water" on top of the hole
           glTranslatef(0, -0.3f, 0); // Water on top (but still a bit below normal level)
           glRotatef(moonRot, 0,1,0);
            glBindTexture(GL_TEXTURE_2D, lake.texture_water);
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
        glRotatef(- moonRot * 0.4, 0, 1, 0);
        skyline.drawClouds();
    glPopMatrix();
    skyline.draw();
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
