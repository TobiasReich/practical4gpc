#ifndef WORLD_H
#define WORLD_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>

#include <GL/freeglut.h>
#include <GL/gl.h>


using namespace std;

class World
{
    public:
        /** Struct for defining a position on the level (2D) **/
        struct Position2D{
            float lat, lon;
        };


        std::vector<World::Position2D> treeSmallPositions;
        std::vector<World::Position2D> treeNormalPositions;
        std::vector<World::Position2D> treeBigPositions;

        Position2D lakePos = {10, 10};// Position of the lake
        Position2D fountainPos = {25, -10};// Position of the fountain (imported Object)

        float moonRot = 0;                  // Rotation of the moon (in degrees)

        Position2D housePosition = {-10, 10};
        Position2D cam1Position = {-49, 24};
        Position2D cam2Position = {-49, -24};
        Position2D cam3Position = {49, 24};
        Position2D cam4Position = {49, -24};
        /* the destination where to look with all cams
            This can be simply one value. If it rotates around an epicentre
            of the world with a radius big enough for all cameras to move.
            Thus, especially since all cameras have the same distance to the center,
            forms concentric movement for all cameras but requires only one calculation
            for the destination.
        */
        Position2D camDest = {0, 0};
        float camRotation = 0;

        // LIGHTs
        bool moonLight = true;            // scene light on

        GLuint texture_fence;               // texture for the fence
        GLuint texture_player_character;     // texture for the background of the minimap

        const char * TEXTURE_FENCE_FILENAME     = "Data/gfx/fence.png";
        const char * TEXTURE_PLAYER_FILENAME      = "Data/gfx/player_face.png";

        const float FogColor[3]={0.2f,0.3f,0.5f}; // Define a nice light grey

        // Methods #############################################
        World();
        virtual ~World();

        void loadWorld(void);
        void drawWorld(void);

        bool checkCollision(float newX, float newZ);
        void updateCameras(void);
        static void resetGLColor(void);        // Resets the GL_DIFFUSE color to "default" values
    protected:
    private:
        // OBJECT GENERATION and World drawing
       // void createImportedObjectDL(void);
        void initTreePositions(void);

        void drawStars(void);
        void drawFountain(void);
        void drawHouse(void);
        void drawTowers(void);
        void drawMoon(void);
        void drawGround(void);
        void drawFence(void);
        void drawTrees(void);
        void drawLake(void);
        void drawSkyline(void);

        void setMoonLighting(void);

        // Collision detection
        bool isNearTo(float newX, float newZ, Position2D &target);
        bool isNearToHouse(float newX, float newZ);
};

#endif // WORLD_H
