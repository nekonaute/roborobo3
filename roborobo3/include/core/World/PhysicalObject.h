/*
 *  PhysicalObject.h
 *  roborobo
 *
 *  Created by Nicolas on 9/4/2014.
 *
 */

#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include "RoboroboMain/common.h"
#include "Utilities/Geometry.h"

class PhysicalObject
{
protected :
    int _id;
    
    int type;
    
    // coordinates
    Point2d _position;
    Sint16 _xCenterPixel;
    Sint16 _yCenterPixel;
    
    Uint8 _displayColorRed;
    Uint8 _displayColorGreen;
    Uint8 _displayColorBlue;
    
    int regrowTimeMax; // max iterations before object re-appear - load from properties file.
    int regrowTime; // iterations before object re-appear
    bool regrow;  // is currently regrowing? (ie. hidden)
    bool relocate; // relocate object when re-appear
    bool overwrite; // write object even if another object/robot will be overwritten.
    
    bool _visible; // display option (default: true)
    
    void init();  // called by constructor only
    
protected:
    int findRandomLocation( );
    void setType ( int __type ) { type = __type; }
    
public :
    
    PhysicalObject( int __id ); // use PhysicalObjectFactory instead!
    ~PhysicalObject() { }

    int getId()
    {
        return _id;
    }
    
    void setPosition(Point2d position)
    {
        _position = position;
        _xCenterPixel = position.x;
        _yCenterPixel = position.y;
    }
    
    Point2d getPosition()
    {
        return _position;
    }
    
    virtual void step() = 0;
    void stepPhysicalObject(); // default step method. Suggested: call this method from step().
    
    virtual bool canRegister() = 0; // test if register object is possible (use both shape or footprints)
    virtual void registerObject() = 0; // register object in the world (write images)
    virtual void unregisterObject() = 0; // unregister object in the world (write blank pixels)

    virtual void isTouched( int __idAgent ) = 0; // callback, triggered by agent
    virtual void isWalked( int __idAgent ) = 0; // callback, triggered by agent
    
    virtual void display() = 0;
    virtual void hide() = 0;
    
    int getType() { return type; }

    static bool isInstanceOf ( int index );
    
};

#endif
