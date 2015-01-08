/*****************************************************/
/*                                                   */
/*             - PRACTICA 4 -                        */
/*              Dayane  LIMA                         */
/*            Tatiane RODRIGUES                      */
/*              Tobias REICH                         */
/*                                                   */
/*****************************************************/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <GL/freeglut.h>
#include <IL/ilut.h>
#include <IL/il.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "Game.h"
#include "Player.h"         // Class responsible for sound & music
#include "SoundManager.h"   // Class responsible for sound & music
#include "World.h"          // World Class
#include "MainGL.h"         // Header for MainGL with used methods

// ------- DEFINITIONS -------

#define PI 3.14159265
#define FPS 25;
#define FRAME_DURATION 1000 / FPS;

using namespace std;


void init(void){
    glClearColor(0.1, 0.1, 0.2f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    // Set Color Material to Change separate Material properties
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor3f(0,0,0);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3f(1,1,1);
    world.resetGLColor();   // Resets the GL_DIFFUSE color

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    width = glutGet(GLUT_WINDOW_WIDTH);
    height  = glutGet(GLUT_WINDOW_HEIGHT);

    // Loading Textures before loading the world!
    initTextures();

    // Load the world (Needs textures already loaded)
    world.loadWorld();

    // Make the cursor invisible
    glutSetCursor(GLUT_CURSOR_NONE);

    // Ignore repeated Keyboard pressing
    glutIgnoreKeyRepeat(true);

    player.setPlayerToStartCoords();
    createPlayerModelDL();
}

/** Creates the Display List for drawing the player model (when seen from camera) **/
void createPlayerModelDL(void){
    GLUquadricObj *playerSphere = gluNewQuadric();
    gluQuadricDrawStyle(playerSphere, GLU_FILL);
    gluQuadricTexture(playerSphere, TRUE);
    gluQuadricNormals(playerSphere, GLU_SMOOTH);

    playerModelDL = glGenLists(1);
    glNewList(playerModelDL, GL_COMPILE);

        glColorMaterial(GL_FRONT, GL_EMISSION);
        glColor3f(10,10,10);   // Reset the original diffuse material value

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, world.texture_player_character);

            gluSphere(playerSphere, 1, 24, 24);

        glColor3f(0,0,0);   // Reset the original diffuse material value
        glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEndList();
    gluDeleteQuadric(playerSphere);
}




/** "Loop" function that is called each time the scene is drawn (each frame) **/
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset the color material each draw call
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glColor3f(1.0f, 1.0f, 1.0f);

    processKeypressed();       // process all currently pressed keys
    soundManager.playCharacterFootsteps();  // reacts to the pressed keys

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    world.updateCameras();      // sets the GL Camera according to the chosen Cam in game
    setCameraPosition();    // updates all "automatic" camera positiones (C1->CG)

    world.drawWorld();

    drawHUD();              // Call this after drawing the scene -> draws the HUD

    // "Pseudo VSync":
    // @see: http://stackoverflow.com/questions/589064/how-to-enable-vertical-sync-in-opengl
    glFlush();
    glFinish();
    glutSwapBuffers();

    waitForFPS();           // Last call, waiting for the fps
}



// sets the GL Camera according to the chosen Cam in game
void setCameraPosition(void){

    if (camNumber == Game::CAM_PLAYER){ // Player cam
        // Easier way of calculating the lookAt-Direction
        float lookAtX = player.eyeX + sin(player.rotLeftRight * PI / 180.0);
        float lookAtY = player.eyeY + sin(player.rotUpDown * PI / 180.0);
        float lookAtZ = player.eyeZ + cos(player.rotLeftRight* PI / 180.0);

        camX = player.eyeX; camY = player.eyeY; camZ = player.eyeZ;
        camAtX = lookAtX; camAtY = lookAtY; camAtZ = lookAtZ;
        camUpX = player.upX; camUpY = player.upY; camUpZ = player.upZ;
    } else if (camNumber == Game::CAM_TOWER_1){
        camX = world.cam1Position.lat; camY = 10; camZ = world.cam1Position.lon;
        camAtX = world.camDest.lat; camAtY = 0; camAtZ = world.camDest.lon;
        camUpX = 0; camUpY = 1; camUpZ = 0;
    } else if (camNumber == Game::CAM_TOWER_2){
        camX = world.cam2Position.lat; camY = 10; camZ = world.cam2Position.lon;
        camAtX = world.camDest.lat; camAtY = 0; camAtZ = world.camDest.lon;
        camUpX = 0; camUpY = 1; camUpZ = 0;
    } else if (camNumber == Game::CAM_TOWER_3){
        camX = world.cam3Position.lat; camY = 10; camZ = world.cam3Position.lon;
        camAtX = world.camDest.lat; camAtY = 0; camAtZ = world.camDest.lon;
        camUpX = 0; camUpY = 1; camUpZ = 0;
    }  else if (camNumber == Game::CAM_TOWER_4){
        camX = world.cam4Position.lat; camY = 10; camZ = world.cam4Position.lon;
        camAtX = world.camDest.lat; camAtY = 0; camAtZ = world.camDest.lon;
        camUpX = 0; camUpY = 1; camUpZ = 0;
    }  else if (camNumber == Game::CAM_HOUSE){
        camX = world.housePosition.lat; camY = 10; camZ = world.housePosition.lon;
        camAtX = world.camDest.lat + world.housePosition.lat; camAtY = 0; camAtZ = world.camDest.lon + world.housePosition.lon;
        camUpX = 0; camUpY = 1; camUpZ = 0;
    }

    gluLookAt(camX, camY, camZ,
                camAtX, camAtY, camAtZ,
                camUpX, camUpY, camUpZ);

    // If not 1st person -> draw the "player model"
    if (camNumber != Game::CAM_PLAYER)
        drawPlayerCharacter();

    /* Sets the position of the light. Now it is always transformed with the world but
    with corrected values if we see the players cam.
    Using this method we may also see the light of the player on the right spot when
    we switch to another camera */
    player.setPlayerLight();
}

/** draws a "character" at the player position
This is used when switching to another camera to "see the player" in the camera picture.
This is not used while 1st person view **/
void drawPlayerCharacter(void){
    glPushMatrix();
        glTranslatef(player.eyeX, player.eyeY, player.eyeZ);
        glRotatef(player.rotLeftRight, 0, 1, 0);
        glRotatef(player.rotUpDown, -1, 0, 0);
        glCallList(playerModelDL);
    glPopMatrix();
}

// User Input -------------------


/** This function is processing keyboard DOWN events **/
void keyboardPressed(unsigned char key, int x, int y){
    switch(key) {
        case 27:  /* escape */
            exit(0);
            break;
        case 'W':
        case 'w':
            wPressed = true;
            sPressed = false;
            break;
        case 'S':
        case 's':
            sPressed = true;
            wPressed = false;
            break;
        case 'A':
        case 'a':
            aPressed = true;
            dPressed = false;
            break;
        case 'D':
        case 'd':
            dPressed = true;
            aPressed = false;
            break;
        case '1':
            world.moonLight = !world.moonLight;
            break;
        case '2':
            player.playerLight = ! player.playerLight;
            break;
        case '3':
            cout << "WRITING SCREENSHOT" << endl;
            takeScreenshot("Screenshot.png");
            break;
        case 'P':
        case 'p':
            showMinimap = !showMinimap;
            break;
        case 'H':
        case 'h':
            showInfo = !showInfo;
            break;
        case 'K':
        case 'k':
            showCompass = !showCompass;
            break;
        case 'C':
        case 'c':
            camNumber = ++camNumber % 6;    // 4 towers, 1 house, 1 "player cam"
            break;
        case 'N':
        case 'n':
            soundManager.playSound = !soundManager.playSound;
            break;
        case 'M':
        case 'm':
            soundManager.playMusic = !soundManager.playMusic;

            if (soundManager.playMusic){
                Mix_ResumeMusic();
            } else {
            Mix_PauseMusic();
            }
            break;
        case 'R':
        case 'r':
            player.setPlayerToStartCoords();
            break;
    }
}

/** This function is processing keyboard RELEASE events **/
void keyboardReleased(unsigned char key, int x, int y){
    switch(key) {
        case 'w':
        case 'W':
            wPressed = false;
            break;
        case 's':
        case 'S':
            sPressed = false;
            break;
        case 'a':
        case 'A':
            aPressed = false;
            break;
        case 'd':
        case 'D':
            dPressed = false;
            break;
    }
}

/** This function is processing "special" events (Cursor keys) **/
void specialInputDOWN(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_UP:
            uPPressed = true;
            downPressed = false;
            break;
        case GLUT_KEY_DOWN:
            uPPressed = false;
            downPressed = true;
            break;
        case GLUT_KEY_LEFT:
            leftPressed = true;
            rightPressed = false;
            break;
        case GLUT_KEY_RIGHT:
            leftPressed = false;
            rightPressed = true;
            break;
    }
}

/** This function is processing "special" events (Cursor keys) **/
void specialInputUP(int key, int x, int y){
    switch(key) {
        case GLUT_KEY_UP:
            uPPressed = false;
            break;
        case GLUT_KEY_DOWN:
            downPressed = false;
            break;
        case GLUT_KEY_LEFT:
            leftPressed = false;
            break;
        case GLUT_KEY_RIGHT:
            rightPressed = false;
            break;
    }
}

/** This function is processing the passive (without
clicking a button) mouse movement and rotates the camera **/
void mousePassiveMovement(int x, int y){
    int deltaX = x - lastMouseX;
	int deltaY = y - lastMouseY;

	lastMouseX = x;
	lastMouseY = y;

    if( deltaX == 0 && deltaY == 0 ) return;

	if (deltaX < -MOUSE_THRESHOLD){
        player.rotLeftRight += MOUSE_SPEED_HOR;
	} else if (deltaX > MOUSE_THRESHOLD)
        player.rotLeftRight -= MOUSE_SPEED_HOR;

    if (deltaY < -MOUSE_THRESHOLD){
        player.rotUpDown += MOUSE_SPEED_VERT;
	} else if (deltaY > MOUSE_THRESHOLD)
        player.rotUpDown -= MOUSE_SPEED_VERT;

    // Check whether the cursor is on the border of the screen
    int windowX		= glutGet( GLUT_WINDOW_X );
	int windowY		= glutGet( GLUT_WINDOW_Y );
	int screenWidth		= glutGet( GLUT_SCREEN_WIDTH );
	int screenHeight	= glutGet( GLUT_SCREEN_HEIGHT );

	int screenLeft  = -windowX;
	int screenTop   = -windowY;
	int screenRight = screenWidth  - windowX;
	int screenBottom= screenHeight - windowY;

	if( x <= screenLeft+10 || (y) <= screenTop+10
        || x >= screenRight-10 || y >= screenBottom - 10) {
		lastMouseX = 250;
		lastMouseY = 250;
		glutWarpPointer(lastMouseX, lastMouseY);
	}
}

/** This function is processing mouse click events **/
void mouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        player.rotUpDown = 0;      // Set Rotation UP-DOWN (vertical) to center
        break;
    case GLUT_RIGHT_BUTTON:
        player.rotLeftRight = 0;   // Set Rotation LEFT-RIGHT (horizontal) to center
        break;
    }
    // Using mouse wheel to change light intensity
    if ((button == 3) || (button == 4)) { // It's a wheel event
        if (state == GLUT_UP)
            return;   // Disregard redundant GLUT_UP events
        // INCREASE OR DECREASE LIGHT INTENSITY
        if (button == 3) {
            if (player.lightIntensity < 10)
                player.lightIntensity += 0.2;
        } else {
            if (player.lightIntensity > 0.2)
                player.lightIntensity -= 0.2;
        }
    }
}

/** Processes pressed keys.
NOTE: This is not the Keyboard listener.
The Keyboard listeners are just notifying if a key is pressed or released.
This function is processing the pressed keys (so more then one key can be pressed) **/
void processKeypressed() {
    soundManager.isMoving = false;

    if (wPressed) { // FORWARD
        newX = player.eyeX + Player::WALKING_SPEED * sin(player.rotLeftRight * PI / 180.0);
        newZ = player.eyeZ + Player::WALKING_SPEED * cos(player.rotLeftRight * PI / 180.0);
        if (! world.checkCollision(newX, newZ)){
            soundManager.isMoving = true;
            player.eyeX = newX;
            player.eyeZ = newZ;
        } else {
            soundManager.playCharacterHitSound();
        }
    } else if (sPressed) { // BACKWARDS
        newX = player.eyeX - Player::WALKING_SPEED * sin(player.rotLeftRight * PI / 180.0);
        newZ = player.eyeZ - Player::WALKING_SPEED * cos(player.rotLeftRight * PI / 180.0);
        if (! world.checkCollision(newX, newZ)){
            soundManager.isMoving = true;
            player.eyeX = newX;
            player.eyeZ = newZ;
        } else {
            soundManager.playCharacterHitSound();
        }
    }
    if (aPressed) { // STRAFE LEFT
        newX = player.eyeX + Player::WALKING_SPEED * cos(player.rotLeftRight * PI / 180.0);
        newZ = player.eyeZ - Player::WALKING_SPEED * sin(player.rotLeftRight * PI / 180.0);
        if (! world.checkCollision(newX, newZ)){
            soundManager.isMoving = true;
            player.eyeX = newX;
            player.eyeZ = newZ;
        } else {
            soundManager.playCharacterHitSound();
        }
    } else if (dPressed) { // STRAFE RIGHT
        newX = player.eyeX - Player::WALKING_SPEED * cos(player.rotLeftRight * PI / 180.0);
        newZ = player.eyeZ + Player::WALKING_SPEED * sin(player.rotLeftRight * PI / 180.0);
        if (! world.checkCollision(newX, newZ)){
            soundManager.isMoving = true;
            player.eyeX = newX;
            player.eyeZ = newZ;
        } else {
            soundManager.playCharacterHitSound();
        }
    }

    // CURSOR KEYS -------------------------
    if (uPPressed) {             // CURSOR UP
        if (player.rotUpDown < 89)
            player.rotUpDown += Player::ROTATION_SPEED;
    } else if (downPressed) {    // CURSOR DOWN
        if (player.rotUpDown > -89)
            player.rotUpDown -= Player::ROTATION_SPEED;
    }
    if (leftPressed) {           // CURSOR LEFT
        player.rotLeftRight += Player::ROTATION_SPEED;
    } else if (rightPressed) {   // CURSOR RIGHT
        player.rotLeftRight -= Player::ROTATION_SPEED;
    }
}


// Texture Loader stuff ---------


/** @deprecated - use loadTextureDevil() instead!

Always forget the filtering methods:
GL_NEAREST - no filtering, no mipmaps
GL_LINEAR - filtering, no mipmaps
GL_NEAREST_MIPMAP_NEAREST - no filtering, sharp switching between mipmaps
GL_NEAREST_MIPMAP_LINEAR - no filtering, smooth transition between mipmaps
GL_LINEAR_MIPMAP_NEAREST - filtering, sharp switching between mipmaps
GL_LINEAR_MIPMAP_LINEAR - filtering, smooth transition between mipmaps

So:
GL_LINEAR is bilinear
GL_LINEAR_MIPMAP_NEAREST is bilinear with mipmaps
GL_LINEAR_MIPMAP_LINEAR is trilinear
**/
void LoadTexture(const char * filename, GLuint &texture) {
    FILE *file;
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int size;
    unsigned int width, height;
    unsigned char *data;
    file = fopen(filename, "rb");
    if (file == NULL) {
        //MessageBox(NULL, L"Error: Invaild file path!", L"Error", MB_OK);
        return;
    }
    if (fread(header, 1, 54, file) != 54) {
        //MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
        return ;
    }
    if (header[0] != 'B' || header[1] != 'M') {
        //MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
        return ;
    }
    dataPos     = *(int*)&(header[0x0A]);
    size        = *(int*)&(header[0x22]);
    width       = *(int*)&(header[0x12]);
    height      = *(int*)&(header[0x16]);
    if (size == NULL)
        size = width * height * 3;
    if (dataPos == NULL)
        dataPos = 54;
    data = new unsigned char[size];
    fread(data, 1, size, file);
    fclose(file);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Bitmap is stored in BGR Format. GL_BGR is not available in the delivered packages
    // For this reason textures have to be color inverted
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

/** Since the DevIL does all the work for us, this is the new LoadTexture methode **/
void loadTextureDevil(const char * filename, GLuint &texture, bool clamp) {
    // const_cast here for assigning a const char* (string) to a method that
    // requieres a char*
    // Since we won't modify the char* this is safe here
    texture = ilutGLLoadImage(const_cast<char*> (filename));

    if (clamp){
        glBindTexture(GL_TEXTURE_2D, texture);
        // Only needs to be clamped in "height" since textures repeat in width (fence, grass...)
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
}

/**  Initializes the textures and sets their ID's **/
void initTextures(void){
    // World textures
    loadTextureDevil(world.TEXTURE_GROUND_FILENAME, world.texture_ground, false);
    loadTextureDevil(world.TEXTURE_WALL_FILENAME, world.texture_wall, false);
    loadTextureDevil(world.TEXTURE_FENCE_FILENAME, world.texture_fence, true);
    loadTextureDevil(world.TEXTURE_PLANT_FILENAME, world.texture_plant, false);
    loadTextureDevil(world.TEXTURE_GRASS_FILENAME, world.texture_grass, true);
    loadTextureDevil(world.TEXTURE_TRUNK_FILENAME, world.texture_trunk, false);
    loadTextureDevil(world.TEXTURE_HOUSE_WINDOW_FILENAME, world.texture_house_window, false);
    loadTextureDevil(world.TEXTURE_HOUSE_DOOR_FILENAME, world.texture_house_door, false);
    loadTextureDevil(world.TEXTURE_HOUSE_ROOF_FILENAME, world.texture_house_roof, false);
    loadTextureDevil(world.TEXTURE_MOON_FILENAME, world.texture_moon, true);
    loadTextureDevil(world.TEXTURE_PANDA_FILENAME, world.texture_panda, true);

    loadTextureDevil(world.TEXTURE_LAKE_WATER, world.texture_water, true);
    loadTextureDevil(world.TEXTURE_BEACH, world.texture_beach, false);


    loadTextureDevil(world.TEXTURE_SKYLINE_FILENAME, world.texture_skyline, true);
    loadTextureDevil(world.TEXTURE_CLOUDS_FILENAME, world.texture_clouds, false);


    // HUD textures
    loadTextureDevil(TEXTURE_COMPASS_NEEDLE_FILENAME, texture_minimap_compass, true);
    loadTextureDevil(TEXTURE_COMPASS_FILENAME, texture_compass_background, true);
    loadTextureDevil(TEXTURE_PLAYER_SYMBOL_FILENAME, texture_minimap_player, true);
    loadTextureDevil(TEXTURE_TREE_MMAP_FILENAME, texture_minimap_tree, true);
    loadTextureDevil(TEXTURE_MINIMAP_BG_FILENAME, texture_minimap_background, true);
    loadTextureDevil(TEXTURE_MINIMAP_LIGHT, texture_minimap_light, true);
    loadTextureDevil(TEXTURE_MINIMAP_MOON_LIGHT, texture_minimap_moon_light, true);
    loadTextureDevil(TEXTURE_COMPASS_BG_FILENAME, texture_compass_phys_background, true);
    loadTextureDevil(TEXTURE_LAKE_MMAP_FILENAME, texture_minimap_lake, true);
    loadTextureDevil(TEXTURE_FOUNTAIN_MMAP_FILENAME, texture_minimap_fountain, true);

    loadTextureDevil(TEXTURE_REC, texture_rec, true);

    loadTextureDevil(world.TEXTURE_PLAYER_FILENAME, world.texture_player_character, true);


    /*  // This is the self impleented code for loading textures.
        LoadTexture(TEXTURE_GROUND_FILENAME, texture_ground);
    */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // set it once for all
}


// FPS stuff --------------------

/** Empty function - sets the global idle callback. **/
void idle(void){
    glutPostRedisplay();
}

/** Is waiting for the "end of this frame" so the fps is constant **/
void waitForFPS(void){
    Sleep(time_left());
    next_frame += FRAME_DURATION;
}

/** Is calculating the time until the next frame. Returns the milliseconds
This method is used for waiting in case the GPU is faster than the desired fps.**/
GLuint time_left(void) {
    GLuint now = glutGet(GLUT_ELAPSED_TIME);
    if (next_frame <= now)
        return 0;
    else
        return next_frame - now;
}


// HUD stuff ---------------------


/** Switches to orthographic projection and disables light and depth test for drawing in 2D (HUD)**/
void setOrthographicProjection(void) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, width, height, 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}


/** Switches back to perspectice projection to draw the scene **/
void resetPerspectiveProjection() {
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}


/** This draws the fps on the HUD **/
void drawFPS(){
    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
    glRasterPos2i(25, 25);
    char s[25];
    ++frame;
    time=glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        actualFPS = frame*1000.0/(time-timebase);
        timebase = time;
        frame = 0;
    }
    sprintf(s,"Current FPS: %4.2f",actualFPS);
    displayString(s);

    // Light Info
    glRasterPos2i(25, 38);
    if (world.moonLight) {
        if (player.playerLight) {
            char s[] = "Moon = on; Flashlight = on";
            displayString(s);
        } else {
            char s[] = "Moon = on; Flashlight = off";
            displayString(s);
        }
    } else if (player.playerLight) {
        char s[] = "Moon = off; Flashlight = on";
        displayString(s);
    } else {
        char s[] = "Moon = off; Flashlight = off";
        displayString(s);
    }

    glRasterPos2i(25, 51);
    if (soundManager.playSound) {
        if (soundManager.playMusic) {
            displayString("Sound is ON / Music is ON");
        } else {
            displayString("Sound is ON / Music is OFF");
        }
    } else {
        if (soundManager.playMusic) {
            displayString("Sound is OFF / Music is ON");
        } else {
            displayString("Sound is OFF / Music is OFF");
        }
    }
    glRasterPos2i(25, 64);
    char camString[25];
    sprintf(camString,"CAM: %4i",camNumber);
    displayString(camString);
}


/** Draws a crosshair in the center of the scene **/
void drawCrosshair(void){
    glLineWidth(2);
    glDisable(GL_BLEND);
    glColor4f(1, 0, 0, 1);
    glBegin(GL_LINES);
        glVertex2f(width/2.0,   height/2.0-15);
        glVertex2f(width/2.0,   height/2.0+15);
        glVertex2f(width/2.0-15,height/2.0);
        glVertex2f(width/2.0+15,height/2.0);
    glEnd();
}


/** Draws the compass of the player character
this draws a compass background (fixed) first,
then the needle on top of it (rotating) **/
void drawCompass(void){
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glColor4f(1, 1, 1, 1);

    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture_compass_phys_background);
        glTranslatef(width-256, 0, 0.0); // Move compass to the top right
        // Compass background
        glBegin(GL_QUADS);
            glTexCoord2f(0.01, 0.99); glVertex2f(0, 0); // lt
            glTexCoord2f(0.99, 0.99); glVertex2f(256, 0); // rt
            glTexCoord2f(0.99, 0.01); glVertex2f(256, 256);  // rb
            glTexCoord2f(0.01, 0.01); glVertex2f(0, 256);  // lb
        glEnd();
    glPopMatrix();
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture_compass_background);
        glTranslatef(width-100, 100.0, 0.0); // Move compass to the top right
        // Compass background
        glBegin(GL_QUADS);
            glTexCoord2f(0.01, 0.99); glVertex2f(-COMPASS_SIZE, -COMPASS_SIZE); // lt
            glTexCoord2f(0.99, 0.99); glVertex2f( COMPASS_SIZE, -COMPASS_SIZE); // rt
            glTexCoord2f(0.99, 0.01); glVertex2f( COMPASS_SIZE, COMPASS_SIZE);  // rb
            glTexCoord2f(0.01, 0.01); glVertex2f(-COMPASS_SIZE, COMPASS_SIZE);  // lb
        glEnd();

        // Compass needle
        glBindTexture(GL_TEXTURE_2D, texture_minimap_compass);
        glRotatef(- player.rotLeftRight , 0.0, 0.0, 1.0); // Rotate the needle negative to player rotation
        glBegin(GL_QUADS);
            glTexCoord2i(0, 1); glVertex2f(-COMPASS_SIZE, -COMPASS_SIZE); // lt
            glTexCoord2i(1, 1); glVertex2f( COMPASS_SIZE, -COMPASS_SIZE); // rt
            glTexCoord2i(1, 0); glVertex2f( COMPASS_SIZE, COMPASS_SIZE);  // rb
            glTexCoord2i(0, 0); glVertex2f(-COMPASS_SIZE, COMPASS_SIZE);  // lb
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

    glPopMatrix();
}

/** Draws the HUD-stats on the screen.
This excludes the fps counter but more the elements in a sense of a "help" for the user,
keys to press etc. **/
void drawStats() {
    glPushMatrix();
        // Drawing Background
        glTranslatef(25, 75, 0.0); // Move compass to the top right
        glPushMatrix();
            glColor4f(0.25f, 0.25f, 0.25f, 0.5f);
            glEnable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);

            glBegin(GL_QUADS);
                glVertex2f(   0,  0); // lt
                glVertex2f( 280,  0); // rt
                glVertex2f( 280, 215);  // rb
                glVertex2f(   0, 215);  // lb
            glEnd();
            glDisable(GL_BLEND);
        glPopMatrix();

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glRasterPos2i(5, 20);
        displayStringHeadder("Keys:");

        // ROW 1
        glRasterPos2i(5, 40);
        displayString("W: Move forwards");
        glRasterPos2i(5, 55);
        displayString("S: Move backwards");
        glRasterPos2i(5, 70);
        displayString("A: Strafe left");
        glRasterPos2i(5, 85);
        displayString("D: Strafe right");

        glRasterPos2i(5, 100);
        displayString("Cursors / Mouse: Look");
        glRasterPos2i(5, 145);
        displayString("Mouse wheel: Change light intensity");

        glRasterPos2i(5, 160);
        displayString("1: Toggle scene light");
        glRasterPos2i(5, 175);
        displayString("2: Toggle player /flash light");
        glRasterPos2i(5, 190);
        displayString("3: Make Screenshot");

        glRasterPos2i(5, 205);
        displayString("ESC: QUIT");

        // ROW 2
        glRasterPos2i(150, 40);
        displayString("H: Toggle Help");
        glRasterPos2i(150, 55);
        displayString("K: Toggle Compass");
        glRasterPos2i(150, 70);
        displayString("P: Toggle Map");
        glRasterPos2i(150, 85);
        displayString("M: Toggle Music");
        glRasterPos2i(150, 100);
        displayString("N: Toggle Footsteps");
        glRasterPos2i(150, 115);
        displayString("C: Switch Camera");

    glPopMatrix();
 }

/** Draws all Elements of the Minimap. This includes:
- 3 types of trees
- the house
- the lake
- the player
- the fountain
**/
void drawMinimap(){

    glEnable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);


    // Drawing Objects ---------------

    // WALLS & Towers
    glColor4f(0.7f, 0.3f, 0.15f, 1);
    glLineWidth(6);
    glPushMatrix();
        glBegin(GL_LINE_LOOP);
            glVertex2f( width - Game::MMAP_WIDTH -Game::MMAP_MARGIN, height - Game::MMAP_HEIGHT-Game::MMAP_MARGIN);
            glVertex2f( width - Game::MMAP_MARGIN, height - Game::MMAP_HEIGHT-Game::MMAP_MARGIN);
            glVertex2f( width - Game::MMAP_MARGIN, height - Game::MMAP_MARGIN);
            glVertex2f( width - Game::MMAP_WIDTH -Game::MMAP_MARGIN, height - Game::MMAP_MARGIN);
        glEnd();

        glColor3f(0.2f, 0.2f, 0.2f);
        glPointSize(12);

        glBegin(GL_POINTS);
            glVertex2f( width - Game::MMAP_WIDTH -Game::MMAP_MARGIN + 3, height - Game::MMAP_HEIGHT-Game::MMAP_MARGIN + 3);
            glVertex2f( width - Game::MMAP_MARGIN - 3 , height - Game::MMAP_HEIGHT-Game::MMAP_MARGIN + 3);
            glVertex2f( width - Game::MMAP_MARGIN - 3 , height - Game::MMAP_MARGIN - 3);
            glVertex2f( width - Game::MMAP_WIDTH -Game::MMAP_MARGIN + 3, height - Game::MMAP_MARGIN - 3);
        glEnd();
    glPopMatrix();

    // Trees -----
    /** Negative Values for the minimap since we actually look south **/
    // Small tree
    glColor3ub(0, 200, 0);
    glPointSize(4);

    glPushMatrix();
        for (std::size_t i = 0; i< world.treeSmallPositions.size(); ++i){
            World::Position2D tree = world.treeSmallPositions[i];
            glBegin(GL_POINTS);
                glVertex2f( tree.lat * -4 + width - Game::MMAP_WIDTH /2 - Game::MMAP_MARGIN
                           ,tree.lon * -4 + height - Game::MMAP_HEIGHT/2 - Game::MMAP_MARGIN);
            glEnd();
        }
    glPopMatrix();

    // Normal Tree
    glColor4f(1,1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_minimap_tree);

    for (std::size_t i = 0; i< world.treeNormalPositions.size(); ++i){
        World::Position2D tree = world.treeNormalPositions[i];

        glPushMatrix();
            glTranslatef(tree.lat * -4 + width - Game::MMAP_WIDTH /2 - Game::MMAP_MARGIN,
                         tree.lon * -4 + height - Game::MMAP_HEIGHT/2 - Game::MMAP_MARGIN,
                         0.0);

            glBegin(GL_QUADS);
                glTexCoord2i(0, 0); glVertex2f( -8, -8); // lt
                glTexCoord2i(0, 1); glVertex2f( +8, -8); // rt
                glTexCoord2i(1, 1); glVertex2f( +8, +8);  // rb
                glTexCoord2i(1, 0); glVertex2f( -8, +8);  // lb
            glEnd();
        glPopMatrix();
    }

    // Big Tree
    for (std::size_t i = 0; i< world.treeBigPositions.size(); ++i){
        World::Position2D tree = world.treeBigPositions[i];

        glPushMatrix();
            glTranslatef(tree.lat * -4 + width - Game::MMAP_WIDTH /2 - Game::MMAP_MARGIN,
                         tree.lon * -4 + height - Game::MMAP_HEIGHT/2 - Game::MMAP_MARGIN,
                         0.0);

            glBegin(GL_QUADS);
                glTexCoord2i(0, 0); glVertex2f( -12, -12); // lt
                glTexCoord2i(0, 1); glVertex2f( +12, -12); // rt
                glTexCoord2i(1, 1); glVertex2f( +12, +12);  // rb
                glTexCoord2i(1, 0); glVertex2f( -12, +12);  // lb
            glEnd();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);

    // HOUSE
    glPushMatrix();
    glColor4f(1, 0.5f, 0.2f, 1);
        glPointSize(5);

        glTranslatef(world.housePosition.lat * -4 + width - Game::MMAP_WIDTH /2 - Game::MMAP_MARGIN,
                     world.housePosition.lon * -4 + height - Game::MMAP_HEIGHT/2 - Game::MMAP_MARGIN, 0.0); // Move compass to the top right

        glBegin(GL_QUADS);
            glVertex2f( -8, -8); // lt
            glVertex2f( +8, -8); // rt
            glVertex2f( +8, +8);  // rb
            glVertex2f( -8, +8);  // lb
        glEnd();
        world.resetGLColor();

    glPopMatrix();

    // LAKE
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_minimap_lake);

    glPushMatrix();
        glTranslatef(world.lakePos.lat * -4 + width - Game::MMAP_WIDTH /2 - Game::MMAP_MARGIN,
                     world.lakePos.lon * -4 + height - Game::MMAP_HEIGHT/2 - Game::MMAP_MARGIN,
                     0.0);

        glBegin(GL_QUADS);
            glTexCoord2i(0, 1); glVertex2f( -17, -17); // lt
            glTexCoord2i(1, 1); glVertex2f( +17, -17); // rt
            glTexCoord2i(1, 0); glVertex2f( +17, +17);  // rb
            glTexCoord2i(0, 0); glVertex2f( -17, +17);  // lb
        glEnd();
    glPopMatrix();

    // Fountain
    glBindTexture(GL_TEXTURE_2D, texture_minimap_fountain);

    glPushMatrix();
        glTranslatef(world.fountainPos.lat * -4 + width - Game::MMAP_WIDTH /2 - Game::MMAP_MARGIN,
                     world.fountainPos.lon * -4 + height - Game::MMAP_HEIGHT/2 - Game::MMAP_MARGIN,
                     0.0);
        glBegin(GL_QUADS);
            glTexCoord2i(0, 1); glVertex2f( -12, -12);
            glTexCoord2i(1, 1); glVertex2f( +12, -12);
            glTexCoord2i(1, 0); glVertex2f( +12, +12);
            glTexCoord2i(0, 0); glVertex2f( -12, +12);
        glEnd();
    glPopMatrix();


    // Player -----
    float playerX = player.eyeX * -4 + width - Game::MMAP_WIDTH /2 - Game::MMAP_MARGIN;
    float playerZ = player.eyeZ * -4 + height - Game::MMAP_HEIGHT/2 - Game::MMAP_MARGIN;

    glBindTexture(GL_TEXTURE_2D, texture_minimap_player);

    glPushMatrix();
        glTranslatef(playerX, playerZ, 0.0); // Move compass to the top right
        glRotatef(- player.rotLeftRight, 0.0, 0.0, 1.0); // Rotate the needle negative to player rotation

        glBegin(GL_QUADS);
            glTexCoord2i(0, 1); glVertex2f( -7, -7); // lt
            glTexCoord2i(1, 1); glVertex2f( +7, -7); // rt
            glTexCoord2i(1, 0); glVertex2f( +7, +7);  // rb
            glTexCoord2i(0, 0); glVertex2f( -7, +7);  // lb
        glEnd();


       // glDisable(GL_TEXTURE_2D);
    glPopMatrix();


     // Draw the Minimap Background Picture at last so we can see the "reflection" on top
    glBindTexture(GL_TEXTURE_2D, texture_minimap_background);

    glColor3f(1,1,1);
    glPushMatrix();
        glTranslatef(width - 512, height - 512, 0.0);

        glBegin(GL_QUADS);
            glTexCoord2i(0, 1); glVertex2f( 0, 0); // lt
            glTexCoord2i(1, 1); glVertex2f( 512, 0); // rt
            glTexCoord2i(1, 0); glVertex2f( 512, 512);  // rb
            glTexCoord2i(0, 0); glVertex2f( 0, 512);  // lb
        glEnd();
    glPopMatrix();


    // Light bulbs in the HUD (MOON / FLASHLIGHT)
    if (player.playerLight){
        glBindTexture(GL_TEXTURE_2D, texture_minimap_light);

        glPushMatrix();
            glTranslatef(width - 57, height - 235, 0.0);

            glBegin(GL_QUADS);
                glTexCoord2i(0, 1); glVertex2f( 0, 0); // lt
                glTexCoord2i(1, 1); glVertex2f( 64, 0); // rt
                glTexCoord2i(1, 0); glVertex2f( 64, 64);  // rb
                glTexCoord2i(0, 0); glVertex2f( 0, 64);  // lb
            glEnd();
        glPopMatrix();
    }
    if (world.moonLight){
        glBindTexture(GL_TEXTURE_2D, texture_minimap_moon_light);

        glPushMatrix();
            glTranslatef(width - 57, height - 269, 0.0);

            glBegin(GL_QUADS);
                glTexCoord2i(0, 1); glVertex2f( 0, 0); // lt
                glTexCoord2i(1, 1); glVertex2f( 64, 0); // rt
                glTexCoord2i(1, 0); glVertex2f( 64, 64);  // rb
                glTexCoord2i(0, 0); glVertex2f( 0, 64);  // lb
            glEnd();
        glPopMatrix();
    }
}


/** Draws the cam logo if it is not the Player camera! **/
void drawCamLogo(){
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glRasterPos2i(30, height-50);

    switch (camNumber){
    case Game::CAM_TOWER_1 :
        displayStringHeadder("CAM 1 - NORTH-East Tower");
        break;
    case Game::CAM_TOWER_2 :
        displayStringHeadder("CAM 2 - SOUTH-East Tower");
        break;
    case Game::CAM_TOWER_3 :
        displayStringHeadder("CAM 3 - NORTH-West Tower");
        break;
    case Game::CAM_TOWER_4 :
        displayStringHeadder("CAM 4 - SOUTH-West Tower");
        break;
    case Game::CAM_HOUSE :
        displayStringHeadder("House Cam");
        break;
    }

    // Show the REC Symbol
    if(camNumber != Game::CAM_PLAYER){
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_rec);

        glPushMatrix();
            glTranslatef(30, height - 100, 0.0);

            glBegin(GL_QUADS);
                glTexCoord2i(0, 1); glVertex2f( 0, 0); // lt
                glTexCoord2i(1, 1); glVertex2f( 64, 0); // rt
                glTexCoord2i(1, 0); glVertex2f( 64, 32);  // rb
                glTexCoord2i(0, 0); glVertex2f( 0, 32);  // lb
            glEnd();
        glPopMatrix();
    }

}


/** Draws the HUD elements
        1. FPS Counter
        2. Crosshair
        3. Compass
        4. Minimap **/
void drawHUD(void) {
    setOrthographicProjection();

    drawCrosshair();

    drawFPS();

    if (showInfo)
        drawStats();

    if(showCompass)
        drawCompass();

    if (showMinimap)
        drawMinimap();

    if (camNumber != Game::CAM_PLAYER)
        drawCamLogo();

    resetPerspectiveProjection();
}



/** Is writing a text on the screen **/
void displayString(char *s) {
    for (unsigned int i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,s[i]);
    }
}

/** Is writing a text on the screen **/
void displayStringHeadder(char *s) {
    for (unsigned int i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[i]);
    }
}


// ----- System ---------------

/** Takes and writes the screenshot to a file **/
void takeScreenshot(const char* filename){
    ILuint imageID = ilGenImage();
    ilBindImage(imageID);
    ilutGLScreen();
    ilEnable(IL_FILE_OVERWRITE);
    ilSaveImage(filename);
    ilDeleteImage(imageID);
}

/** Function is called each time the
window is reshaped **/
void reshape(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, (float) width/height, PERSPECTIVE_NEAR, PERSPECTIVE_FAR);
}



int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Practica 2 - Dayane Lima, Tobias Reich");
    glutFullScreen();

    // Init grafics
    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
        /* wrong DevIL version -> EXIT*/
        SDL_Quit();
        return -1;
    }
    ilInit();
    iluInit();
    ilutRenderer(ILUT_OPENGL);

    // Init SoundManager
    soundManager.init();

    // Comes after iluInit() so ilut can read the textures
    init();

    // DISPLAY
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // PRESSING & RELEASING KEYBOARD KEYS
    glutKeyboardFunc(keyboardPressed);
    glutKeyboardUpFunc(keyboardReleased);

    // CURSOR KEYS
    glutSpecialFunc(specialInputDOWN);
    glutSpecialUpFunc(specialInputUP);

    // MOUSE CLICK AND MOVEMENT
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mousePassiveMovement);

    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
