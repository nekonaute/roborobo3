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
    
    // the speed we expect to move at after computing collisions, which we need to be able to tell other objects
    // in polar coordinates
    double _desiredSpeedOrientation;
    double _desiredLinearSpeed;

    bool _hitWall; // to monitor move result
    bool _didMove; // to monitor move result
    bool _shifting; // to monitor move result: distance travelled since previous position
    
    double _desiredX; //
    double _desiredY; //

    // the impulses given to the object by nearby robots in the current time step, or other objects in the former
    std::map<int, std::tuple<double, double>> _impulses;
    std::map<int, double> _efforts; // remember how much each robot actually pushed us
    
public :
    
    MovableObject( int __id ); // use PhysicalObjectFactory instead!
    ~MovableObject() { }
    
    bool canRegister();
    bool canRegister( Sint16 __x, Sint16 __y ); // can we register the object at that position

    void step();
    void move();
    void isTouched( int __idAgent ); // callback, triggered by agent
    void isWalked( int __idAgent ); // callback, triggered by agent
    void isPushed( int __id, std::tuple<double, double> __speed ); // callback, triggered by collision w/ agent/object
};

#endif
