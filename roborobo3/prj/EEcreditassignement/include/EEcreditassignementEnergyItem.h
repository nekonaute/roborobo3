//
//  EEcreditassignementEnergyItem.hpp
//  roborobo3
//
//  Created by Nicolas Bredeche on 06/12/17.
//  Copyright © 2017 Nicolas Bredeche. All rights reserved.
//

#ifndef EEcreditassignementEnergyItem_hpp
#define EEcreditassignementEnergyItem_hpp

#include "World/EnergyItem.h"

class EEcreditassignementEnergyItem : public EnergyItem
{
private:
    
    double _offsetRegion;

public :
    
    EEcreditassignementEnergyItem( int __id );
    ~EEcreditassignementEnergyItem() { }
    
    void step();
    void isTouched( int __idAgent ); // callback, triggered by agent
    void isWalked( int __idAgent ); // callback, triggered by agent
    void isPushed( int __id, std::tuple<double, double> __speed ); // callback, triggered by collision w/ agent/object
    
    void relocate();
};

#endif /* EEcreditassignementEnergyItem_hpp */
