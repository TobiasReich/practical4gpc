#ifndef GROUNDPATCH_H
#define GROUNDPATCH_H

#include <GameObject.h>

#include <GL/freeglut.h>

class GroundPatch : public GameObject
{
    public:
        GLuint groundPatchDL;               // The Display-List for the ground patches
        const char * TEXTURE_GROUND_FILENAME    = "Data/gfx/ground.png";
        GLuint texture_ground;              // texture for the ground

        GroundPatch();
        virtual ~GroundPatch();

        void create(void) override;
        void draw(void) override;
    protected:
    private:
};

#endif // GROUNDPATCH_H
