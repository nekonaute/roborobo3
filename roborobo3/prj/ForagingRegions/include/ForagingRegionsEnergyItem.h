//
//  ForagingRegionsEnergyItem.hpp
//  roborobo3
//
//  Created by Nicolas Bredeche on 06/12/17.
//  Copyright © 2017 Nicolas Bredeche. All rights reserved.
//

#ifndef ForagingRegionsEnergyItem_hpp
#define ForagingRegionsEnergyItem_hpp

#include "World/EnergyItem.h"

class ForagingRegionsEnergyItem : public EnergyItem
{
private:
    
    double _offsetRegion;
    int activeIt;

public :
    
    ForagingRegionsEnergyItem( int __id );
    ~ForagingRegionsEnergyItem() { }
    
    void step();
    void isTouched( int __idAgent ); // callback, triggered by agent
    void isWalked( int __idAgent ); // callback, triggered by agent
    void isPushed( int __id, std::tuple<double, double> __speed ); // callback, triggered by collision w/ agent/object
    
    void setOffset( double value );
    virtual void relocate();
};

#endif /* ForagingRegionsEnergyItem_hpp */
