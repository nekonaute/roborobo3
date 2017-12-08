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

double leftRegionDensity = 0.99;

EEcreditassignementEnergyItem::EEcreditassignementEnergyItem( int __id ) : EnergyItem( __id )
{
    if ( random() < leftRegionDensity ) // proportion in left/right parts of environment
        _offsetRegion = 0;
    else
        _offsetRegion = 0.5;
}

void EEcreditassignementEnergyItem::step()
{
    EnergyItem::step();
    //if ( gWorld->getIterations() % (400*50) == 0 )
    //    _leftSideDensity = 1 - _leftSideDensity;
}

void EEcreditassignementEnergyItem::isTouched( int __idAgent )
{
    EnergyItem::isTouched(__idAgent);
}

void EEcreditassignementEnergyItem::isWalked( int __idAgent )
{
    EnergyItem::isWalked(__idAgent);

    relocate();
    _visible = true;
}

void EEcreditassignementEnergyItem::isPushed( int __id, std::tuple<double, double> __speed )
{
    EnergyItem::isPushed(__id,__speed);
}

void EEcreditassignementEnergyItem::relocate()
{
    // * pick new coordinate
    
    unregisterObject();
    
    int border = 40;
    
    //double pi = atan(1)*4;
    
    do{
        
        double xPos;
        
        xPos = random() * 0.5 + _offsetRegion;
        
        // with a sigmoid
        //double value = random();
        //xPos = (1/(1 + std::exp(-value*5 + 13)))*3000; // (1/(1 + Exp[-x*5 + 13])*3000) <===
        //xPos = std::pow(value,3); // x^3   <==
        //xPos = (1/(1 + std::exp(-value*5 + 5)))*2; // (1/(1 + Exp[-x*5 + 5]))
        //xPos = 1 - ( 1 / (1 + std::exp( -value * 10 + 5))); // 1 - (1/(1 + Exp[-x*10 + 5]))
        
        // with a gaussian
        //double sigma = 0.2;
        //double gaussianPeakValue = 1.0 / std::sqrt( 2. * pi * std::pow(sigma,2) );
        //xPos = sigma*randgaussian() / gaussianPeakValue;
        
        double x = xPos * ( gScreenWidth - 2*border ) + border;
        double y = random() * ( gScreenHeight - 2*border ) + border;
        
        setCoordinates(x,y);
        
    } while ( canRegister() == false );
    
    registerObject();
}
