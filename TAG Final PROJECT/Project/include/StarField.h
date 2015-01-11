#ifndef STARFIELD_H
#define STARFIELD_H

#include <GameObject.h>
#include <GL/freeglut.h>

class StarField : public GameObject
{
    public:
        GLuint starsDL;                     // The Display-List for the stars

        StarField();
        virtual ~StarField();

        void create(void) override;
        void draw(void) override;
    protected:
    private:
};

#endif // STARFIELD_H
