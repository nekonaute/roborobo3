/*
 *  GateObject.h
 *  roborobo
 *
 *  Created by Nicolas on 26/4/2014.
 *
 */

#ifndef GATEOBJECT_H
#define GATEOBJECT_H

#include "RoboroboMain/common.h"
#include "Utilities/Geometry.h"

#include "World/SquareObject.h"

class GateObject : public SquareObject
{

private:
    
public :
    
    GateObject( int __id ); // use PhysicalObjectFactory instead!
    ~GateObject() { }

    void step();
    void isTouched( int __idAgent ); // callback, triggered by agent
    void isWalked( int __idAgent ); // callback, triggered by agent
};

#endif
