/*
 *  SwitchObject.h
 *  roborobo
 *
 *  Created by Nicolas on 25/4/2014.
 *
 */

#ifndef SWITCHOBJECT_H
#define SWITCHOBJECT_H

#include "RoboroboMain/common.h"
#include "Utilities/Geometry.h"

#include "World/CircleObject.h"

class SwitchObject : public CircleObject
{

private:
    int sendMessageTo;
    
public :
    
    SwitchObject( int __id ); // use PhysicalObjectFactory instead!
    ~SwitchObject() { }

    void step();
    void isTouched( int __idAgent ); // callback, triggered by agent
    void isWalked( int __idAgent ); // callback, triggered by agent
};

#endif
