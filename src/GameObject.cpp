/** GAME OBJECT
This base class represents a drawable object.
All drawable objects (trees, house etc.) inherit from this class.
Most of all this class offers a draw method which can be called from
the possessing object (e.g. the World).
This way the World can call every object to draw itself without caring
about the details about it's representation **/

#include "GameObject.h"

GameObject::GameObject()
{
    //ctor
}

GameObject::~GameObject()
{
    //dtor
}


void GameObject::draw()
{
    // DRAW ME
}
