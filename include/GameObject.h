#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


class GameObject
{
    public:
        GameObject();
        virtual ~GameObject();
        virtual void draw(void);
        virtual void create(void);
    protected:
    private:
};

#endif // GAMEOBJECT_H
