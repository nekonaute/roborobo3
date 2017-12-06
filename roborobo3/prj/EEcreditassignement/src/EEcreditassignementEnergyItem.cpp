//
//  EEcreditassignementEnergyItem.cpp
//  roborobo3
//
//  Created by Nicolas Bredeche on 06/12/17.
//  Copyright © 2017 Nicolas Bredeche. All rights reserved.
//

#include "EEcreditassignement/include/EEcreditassignementEnergyItem.h"
#include "RoboroboMain/roborobo.h"
#include "World/World.h"
#include "WorldModels/RobotWorldModel.h"
#include "Utilities/Misc.h"

EEcreditassignementEnergyItem::EEcreditassignementEnergyItem( int __id ) : EnergyItem( __id )
{
}

void EEcreditassignementEnergyItem::step()
{
    EnergyItem::step();
}

void EEcreditassignementEnergyItem::isTouched( int __idAgent )
{
    EnergyItem::isTouched(__idAgent);
}

void EEcreditassignementEnergyItem::isWalked( int __idAgent )
{
    EnergyItem::isWalked(__idAgent);
}

void EEcreditassignementEnergyItem::isPushed( int __id, std::tuple<double, double> __speed )
{
    EnergyItem::isPushed(__id,__speed);
}

