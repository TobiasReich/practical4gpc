#ifndef TREES_H
#define TREES_H

#include <GameObject.h>
#include <GL/freeglut.h>

/** Class for drawing different types of trees (so far small, medium and big) **/
class Trees : public GameObject
{
    public:
        GLuint treeSmallDL;                 // The Display-List for the small trees
        GLuint treeNormalDL;                // The Display-List for the normal trees
        GLuint treeBigDL;                   // The Display-List for the big trees

        GLuint texture_grass;               // texture for the grass
        GLuint texture_plant;               // texture for the tree top
        GLuint texture_trunk;               // texture for the trunk
        const char * TEXTURE_PLANT_FILENAME     = "Data/gfx/plant.png";
        const char * TEXTURE_TRUNK_FILENAME     = "Data/gfx/trunk.png";
        const char * TEXTURE_GRASS_FILENAME     = "Data/gfx/grass.png";

        Trees();
        virtual ~Trees();
        void create(void) override;
        // void draw(void) override; // Separated into separate Trees for diversity

        void drawSmallTree(void);
        void drawMediumTree(void);
        void drawBigTree(void);
    protected:
    private:
        void createSmallTree(GLUquadricObj *trunk, GLUquadricObj *sphere);
        void createMediumTree(GLUquadricObj *trunk, GLUquadricObj *spher);
        void createBigTree(GLUquadricObj *trunk, GLUquadricObj *spher);
};

#endif // TREES_H
