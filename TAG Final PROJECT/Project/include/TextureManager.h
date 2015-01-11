#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <GL/freeglut.h>
#include <IL/ilut.h>
#include <IL/il.h>
#include <GL/gl.h>
#include <GL/glext.h>

class TextureManager
{
    public:
        TextureManager();
        virtual ~TextureManager();
        static void loadTextureDevil(const char * filename, GLuint &texture, bool clamp);
    protected:
    private:
};

#endif // TEXTUREMANAGER_H
