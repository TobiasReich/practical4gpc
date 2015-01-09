#ifndef MOON_H
#define MOON_H

#include "GameObject.h"

#include "TextureManager.h"
#include <GL/gl.h>

class Moon : public GameObject {
    public:
        GLuint texture_moon;                // texture for the moon in the sky
        const char * TEXTURE_MOON_FILENAME       = "Data/gfx/moon.png";
        GLuint moonDL;                      // The Display-List for the moon

        Moon();
        virtual ~Moon();
        void create(void) override;
        void draw(void) override;
    protected:
    private:
};

#endif // MOON_H
