/*
 *  RoundObject.h
 *  roborobo
 *
 *  Created by Nicolas on 25/4/2014.
 *
 */

#ifndef ROUNDOBJECT_H
#define ROUNDOBJECT_H

#include "RoboroboMain/common.h"
#include "Utilities/Geometry.h"

#include "World/CircleObject.h"

class RoundObject : public CircleObject
{
private:
    
public :
    
    RoundObject( int __id ); // use PhysicalObjectFactory instead!
    ~RoundObject() { }
    
    void step();
    void isTouched( int __idAgent ); // callback, triggered by agent
    void isWalked( int __idAgent ); // callback, triggered by agent
    
};

#endif
