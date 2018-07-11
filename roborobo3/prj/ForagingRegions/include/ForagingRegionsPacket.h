/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGREGIONSPACKET_H
#define FORAGINGREGIONSPACKET_H

#include "Utilities/Packet.h"

class ForagingRegionsPacket : public Packet
{
    private:

    public:
    
    int regret = 0;
    
    ForagingRegionsPacket() : Packet()
    {
    }
    
    void display()
    {
        Packet::display();
        std::cout << "\tregret       = " << regret << "\n";
    }

};

#endif
