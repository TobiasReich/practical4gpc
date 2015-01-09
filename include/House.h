#ifndef HOUSE_H
#define HOUSE_H

#include "GameObject.h"

#include <GL/freeglut.h>


class House : public GameObject
{
    public:
        House();
        virtual ~House();

        void create(void) override;
        void draw(void) override;
    protected:
    private:
};

#endif // HOUSE_H
