#include "World/MovableObject.h"
#include "RoboroboMain/roborobo.h"

MovableObject::MovableObject( int __id ) : CircleObject( __id ) // should only be called by PhysicalObjectFactory
{
    setType(4);
}

void MovableObject::step()
{
    if ( impulses.size() != 0 )
        move();
    
    //stepPhysicalObject();
    if ( _visible && gPhysicalObjectsRedraw == true )
    {
        registerObject();
    }
}

void MovableObject::move()
{
    // not implemented.
}

void MovableObject::isTouched( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (round object) touched by robot #" << __idAgent << std::endl;
}

void MovableObject::isWalked( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (round object) walked upon by robot #" << __idAgent << std::endl;
}

void MovableObject::isPushed( int __id )
{
    //    if ( gVerbose && gDisplayMode <= 1)
    //        std::cout << "[DEBUG] Physical object #" << this->getId() << " (movable object) walked upon by robot/agent #" << __id << std::endl;
}

