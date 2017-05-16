/*
 *  MovableObject.h
 *  roborobo
 *
 *  Created by Nicolas on 16/5/2017. Original implementation: Theotime
 *
 */

#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H

#include "World/CircleObject.h"

class MovableObject : public CircleObject
{
private:
    
public :
    
    MovableObject( int __id ); // use PhysicalObjectFactory instead!
    ~MovableObject() { }
    
    void step();
    void move();
    void isTouched( int __idAgent ); // callback, triggered by agent
    void isWalked( int __idAgent ); // callback, triggered by agent
    void isPushed( int __id ); // callback, triggered by collision w/ agent/object
};

#endif
