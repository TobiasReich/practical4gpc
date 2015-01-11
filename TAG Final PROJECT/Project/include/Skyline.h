#ifndef SKYLINE_H
#define SKYLINE_H

#include <GameObject.h>

#include <GL/freeglut.h>

class Skyline : public GameObject
{
    public:
        GLuint texture_skyline;             // texture for the "skyline"
        GLuint texture_clouds;              // texture for the clouds
        const char * TEXTURE_SKYLINE_FILENAME      = "Data/gfx/skyline.png";
        const char * TEXTURE_CLOUDS_FILENAME      = "Data/gfx/clouds.png";
        GLuint cloudsDL;                   // The Display-List for the clouds (CUBE)
        GLuint skylineDL;                  // The Display-List for the skyline (trees)

        Skyline();
        virtual ~Skyline();
        void create(void) override;
        void draw(void) override;
        void drawClouds(void);
    protected:
    private:
};

#endif // SKYLINE_H
