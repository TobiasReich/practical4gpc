#ifndef TEST_H
#define TEST_H

// function declarations
void processKeypressed(void);
void LoadTexture(const char * filename, GLuint &texture); //@deprecated
void initTextures(void);            // Loads all textures and sets the IDs
void loadTextureDevil(const char * filename, GLuint &texture, bool clamp);
void waitForFPS(void);              // Manages the fps and waiting
GLuint time_left(void);             // returns the amount of time to the next frame
void updateCameraPositions(void);   // updates all "automatic" camera positiones (C1->CG)
void setCameraPosition(void);       // sets the GL Camera according to the chosen Cam in game
void drawPlayerCharacter(void);     // draws a "character" at the player position
void createPlayerModelDL(void);     // creates the player model Display List

// HUD methods
void setOrthographicProjection(void);
void resetPerspectiveProjection(void);
void drawHUD(void);         // Draws the HUD elements
void drawCrosshair(void);   // Draws the crosshair in the center
void displayString(char *s);// draws a String in OpenGL
void displayStringHeadder(char *s); // writes a headder (bigger)
void drawCompass(void);     // draws the the player's compass
void drawStats(void);       // draws the stats
void drawMinimap(void);     // draws the minimap
void drawCamLogo(void);     // draws a "character" at the player position

void takeScreenshot(const char* filename);


// Textures for HUD (NOT WORLD) -------------------------------------------------

GLuint texture_minimap_compass;             // texture for the compass needle
GLuint texture_compass_background;  // texture for the compass background
GLuint texture_compass_phys_background;  // texture for the compass background

GLuint texture_minimap_player;      // texture for the compass background
GLuint texture_minimap_tree;        // texture for the tree in the minimap
GLuint texture_minimap_lake;        // texture for the lake in the minimap
GLuint texture_minimap_fountain;    // texture for the fountain (Import Object) in the minimap
GLuint texture_minimap_background;  // texture for the background of the minimap
GLuint texture_minimap_light;       // texture for the background of the minimap
GLuint texture_minimap_moon_light;       // texture for the background of the minimap
GLuint texture_rec;       // texture for the background of the minimap


const char * TEXTURE_MINIMAP_BG_FILENAME = "Data/gfx/minimap_background.png";
const char * TEXTURE_MINIMAP_LIGHT       = "Data/gfx/lamp_yellow.png";
const char * TEXTURE_MINIMAP_MOON_LIGHT  = "Data/gfx/lamp_blue.png";
const char * TEXTURE_COMPASS_NEEDLE_FILENAME = "Data/gfx/compass_needle.png";
const char * TEXTURE_COMPASS_FILENAME   = "Data/gfx/compass_background.png";
const char * TEXTURE_PLAYER_SYMBOL_FILENAME = "Data/gfx/playerSymbol.png";
const char * TEXTURE_TREE_MMAP_FILENAME = "Data/gfx/tree_mini_map.png";
const char * TEXTURE_COMPASS_BG_FILENAME = "Data/gfx/physical_compass_background.png";
const char * TEXTURE_LAKE_MMAP_FILENAME = "Data/gfx/lake_mini_map.png";
const char * TEXTURE_FOUNTAIN_MMAP_FILENAME = "Data/gfx/fountain.png";

const char * TEXTURE_REC = "Data/gfx/rec.png";



GLuint playerModelDL;               // DisplayList for the player model (when seen from camera)

// HUD
const int COMPASS_SIZE = 75;        // The size of the player's compass
bool showMinimap = true;
bool showCompass = true;
bool showInfo = true;  // HUD elements




// FPS - values for calculating and printing the fps ------------------
int frame=0, time=0, timebase=0;
GLuint next_frame;                  // Time for the next frame
double actualFPS = 0;


// LIGHTs -------------------------------------------------------------
GLfloat light0SceneDiffuse[]        = {270.0f, 290.0f, 350.0f, 1.0f};
//GLfloat specular1[]                 = {1.5f, 1.5f, 1.5f, 1.0f};


// MOUSE INPUT - last mouse position for mouse view ... ---------------
int lastMouseX = 250;
int lastMouseY = 250;
int MOUSE_THRESHOLD         = 3;             // Threshold for recognizing mouse movement
const float MOUSE_SPEED_HOR = 3.5;   // Speed of mouse movement HORIZONTAL
const float MOUSE_SPEED_VERT= 2.5;  // Speed of mouse movement VERTICAL

// PLAYER MOVEMENT ----------------------------------------------------


// Keyboard Pressed Booleans (to have more than one key pressed) ------
bool wPressed, sPressed, aPressed, dPressed = false;
bool uPPressed, downPressed, leftPressed, rightPressed = false;

// VIEW VALUES (CAMERA / PLAYER) ---------------------------------------
int width, height;              // Screen width / height


// Camera positiones -> can be different from player positiones (@see World)
float camX = 10;
float camY = 1.5f;
float camZ = -20;
float camAtX, camAtY, camAtZ = 0;
float camUpX = 0;
float camUpY = 1;
float camUpZ = 0;

int camNumber = 0;              // current camera number

float newX, newZ;               // used for position update and collision check

const float PERSPECTIVE_NEAR = 1;
const float PERSPECTIVE_FAR = 1000;

Player player;
World world;
SoundManager soundManager;


#endif // TEST_H
