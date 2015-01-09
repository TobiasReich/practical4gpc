#include "TextureManager.h"

TextureManager::TextureManager()
{
    //ctor
}

TextureManager::~TextureManager()
{
    //dtor
}

void TextureManager::loadTextureDevil(const char * filename, GLuint &texture, bool clamp) {
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
