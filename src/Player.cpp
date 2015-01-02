#include "Player.h"

#include <GL/freeglut.h>
#include <IL/ilut.h>
#include <IL/il.h>
#include <GL/gl.h>

float const Player::PLAYER_SIZE = 1.1f;      // Player Size for collision calculation
const float Player::WALKING_SPEED = 0.2f;    // Walking speed
const float Player::ROTATION_SPEED = 5.5f;   // Walking speed

Player::Player() {
    //ctor
}

Player::~Player() {
    //dtor
}

/** This sets the player light. Player means here, that the light is fixed to the
position of the player.
If the player walks towards a wall we can see the wall becoming brighter.

Unlike setmoonLighting() this is moving with the GL-MATRIX Stack**/
void Player::setPlayerLight(void){
    GLfloat light1Diffuse[] =  {lightIntensity,
                                lightIntensity,
                                lightIntensity, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
    //glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);

    // Move the light with the player position (NOT THE CAM)
    // So looking from a different cam the light still stays at the position where the player was / is
    GLfloat positionPlayerLight[] = {eyeX , 1.5, eyeZ, 1.0f};

    //glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);
    glLightfv(GL_LIGHT1, GL_POSITION, positionPlayerLight);
    glLightf(GL_LIGHT1,  GL_LINEAR_ATTENUATION, 1.0);
    //glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 2.0);
    if (playerLight)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
}

/** (Re) sets the player to the start position **/
void Player::setPlayerToStartCoords(void){
    eyeX = 10;
    eyeY = 1.5f;
    eyeZ = -20;
    atX, atY, atZ = 0;
    upX = 0;
    upY = 1;
    upZ = 0;
}
