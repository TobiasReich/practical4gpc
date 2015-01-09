#ifndef WORLD_H
#define WORLD_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <GL/freeglut.h>
#include <GL/gl.h>

#include "ASEParser.h"     // Importer for ASE Files

using namespace std;

class World
{
    public:
        // Fields #############################################
        /** Struct for defining a position on the level (2D) **/
        struct Position2D{
            float lat, lon;
        };

        // Imported Object (3Ds - ASE import)
        //const string filename = "Data/objects/fuente.ase";
        const string filename = "Data/objects/objeto.ase";
        //const string filename = "Data/objects/caja.ase";

        std::vector<ASEParser::Vertex> vertices;
        std::vector<ASEParser::Normal> normals;
        std::vector<ASEParser::Face> faces;

        // Display-Lists for faster rendering
        GLuint importObjectDL;              // The Display-List for the imported object

        GLuint starsDL;                     // The Display-List for the stars
        GLuint groundPatchDL;               // The Display-List for the ground patches


        GLuint treeSmallDL;                 // The Display-List for the small trees
        GLuint treeNormalDL;                // The Display-List for the normal trees
        GLuint treeBigDL;                   // The Display-List for the big trees

        GLuint cloudsDL;                   // The Display-List for the skyline (cube)
        GLuint skylineDL;                   // The Display-List for the skyline (cube)

        std::vector<World::Position2D> treeSmallPositions;
        std::vector<World::Position2D> treeNormalPositions;
        std::vector<World::Position2D> treeBigPositions;

        Position2D lakePos = {10, 10};// Position of the lake
        Position2D fountainPos = {15, -10};// Position of the fountain (imported Object)


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

        // Textures

        GLuint texture_trunk;               // texture for the trunk // Flag pole on tower

        GLuint texture_ground;              // texture for the ground
        GLuint texture_fence;               // texture for the fence
        GLuint texture_grass;               // texture for the ground
        GLuint texture_plant;               // texture for the tree top



        GLuint texture_skyline;             // texture for the "skyline"
        GLuint texture_clouds;              // texture for the clouds

        GLuint texture_player_character;     // texture for the background of the minimap

        const char * TEXTURE_GROUND_FILENAME    = "Data/gfx/ground.png";

        const char * TEXTURE_FENCE_FILENAME     = "Data/gfx/fence.png";
        const char * TEXTURE_PLANT_FILENAME     = "Data/gfx/plant.png";
        const char * TEXTURE_TRUNK_FILENAME     = "Data/gfx/trunk.png";
        const char * TEXTURE_GRASS_FILENAME     = "Data/gfx/grass.png";

        const char * TEXTURE_PLAYER_FILENAME      = "Data/gfx/player_face.png";

        const char * TEXTURE_SKYLINE_FILENAME      = "Data/gfx/skyline.png";
        const char * TEXTURE_CLOUDS_FILENAME      = "Data/gfx/clouds.png";

        const float FogColor[3]={0.2f,0.3f,0.5f}; // Define a nice light grey

        // Methods #############################################
        World();
        virtual ~World();

        void loadWorld(void);
        void drawWorld(void);

        bool checkCollision(float newX, float newZ);
        void updateCameras(void);
        void resetGLColor(void);        // Resets the GL_DIFFUSE color to "default" values
    protected:
    private:
        // OBJECT GENERATION and World drawing
        void createGroundPatchDL(void);
        void createImportedObjectDL(void);
        void createHouseDL(void);
        void createTowerDL(void);
        void createMoonDL(void);
        void createStarDL(void);
        void createTreeDLs(void);
        void initTreePositions(void);
        void createLakeDL(void);
        void createSkylineDL(void);

        void drawStars(void);
        void drawImportedObject(void);
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
