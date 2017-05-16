#include "World/RoundObject.h"


RoundObject::RoundObject( int __id ) : CircleObject( __id ) // should only be called by PhysicalObjectFactory
{
    setType(0);
}

void RoundObject::step()
{
    stepPhysicalObject();
}


void RoundObject::isTouched( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (round object) touched by robot #" << __idAgent << std::endl;
}

void RoundObject::isWalked( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (round object) walked upon by robot #" << __idAgent << std::endl;
}


void RoundObject::isPushed( int __id, std::tuple<double, double> __speed )
{
    //    if ( gVerbose && gDisplayMode <= 1)
    //        std::cout << "[DEBUG] Physical object #" << this->getId() << " (round object) pushed by robot #" << __id << std::endl;
}
