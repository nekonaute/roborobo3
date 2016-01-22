/*
 *  SquareObject.h
 *  roborobo
 *
 *  Created by Nicolas on 26/4/2014.
 *
 */

#ifndef SQUAREOBJECT_H
#define SQUAREOBJECT_H

#include "RoboroboMain/common.h"
#include "Utilities/Geometry.h"

#include "World/PhysicalObject.h"

class SquareObject : public PhysicalObject
{

protected:

    
    int _solid_w;
    int _solid_h;
    int _soft_w;
    int _soft_h;
    
public :
    
    SquareObject( int __id ); // use PhysicalObjectFactory instead!
    ~SquareObject() { }
    
    bool canRegister(); // test if register object is possible (use both shape or footprints)
    void registerObject(); // register object in the world (write images)
    void unregisterObject(); // unregister object in the world (write blank pixels)
    void display(); // wrt. screen-rendering
    void hide();    // wrt. screen-rendering
    
};

#endif
