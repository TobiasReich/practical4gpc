#ifndef LAKE_H
#define LAKE_H

#include <GameObject.h>
#include <GL/freeglut.h>

class Lake : public GameObject
{
    public:
        GLuint lakeDL;                      // The Display-List for the lake
        const char * TEXTURE_LAKE_WATER     = "Data/gfx/lake_water.png";
        const char * TEXTURE_BEACH          = "Data/gfx/beach_ground.png";
        GLuint texture_beach;               // texture for the beach ground
        GLuint texture_water;               // texture for the water in the beach

        Lake();
        virtual ~Lake();

        void create(void) override;
        void draw(void) override;
    protected:
    private:
};

#endif // LAKE_H
