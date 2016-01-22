/*
 *  EnergyItem.h
 *  roborobo
 *
 *  Created by Nicolas on 25/4/2014.
 *
 */

#ifndef ENERGYITEM_H
#define ENERGYITEM_H

#include "RoboroboMain/common.h"
#include "Utilities/Geometry.h"

#include "World/CircleObject.h"

class EnergyItem : public CircleObject
{

private:
    double maxEnergyLevel;
    int energyMode;         // 0: give all energy ; 1: give what is asked, fixed respawn delay ; 2: give what is asked, linear respawn time (ie. from what is asked).

public :
    
    EnergyItem( int __id ); // use PhysicalObjectFactory instead!
    ~EnergyItem() { }

    void step();
    void isTouched( int __idAgent ); // callback, triggered by agent
    void isWalked( int __idAgent ); // callback, triggered by agent
};

#endif
