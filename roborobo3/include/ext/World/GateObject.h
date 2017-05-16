/*
 *  GateObject.h
 *  roborobo
 *
 *  Created by Nicolas on 26/4/2014.
 *
 */

#ifndef GATEOBJECT_H
#define GATEOBJECT_H

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
    void isPushed( int __id, std::tuple<double, double> __speed ); // callback, triggered by collision w/ agent/object
};

#endif
