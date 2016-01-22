#include "World/GateObject.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "World/World.h"

#include <iomanip>

GateObject::GateObject( int __id ) : SquareObject( __id ) // should only be called by PhysicalObjectFactory
{
    setType(2);
}

void GateObject::step()
{
    stepPhysicalObject();
}

void GateObject::isTouched( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object item #" << this->getId() << " (gate) touched by robot #" << __idAgent << std::endl;
}

void GateObject::isWalked( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (gate) walked upon by robot #" << __idAgent << std::endl;
    
    unregisterObject();
    hide();
    _visible = false;

    regrow = true;
    regrowTime = regrowTimeMax;
}
