#include "World/SwitchObject.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "World/World.h"

#include <iomanip>

SwitchObject::SwitchObject( int __id ) : CircleObject( __id ) // should only be called by PhysicalObjectFactory
{
    setType(3);

	std::string s = "";
	std::stringstream out;
	out << getId();

    s = "physicalObject[";
	s += out.str();
	s += "].sendMessageTo";
	if ( gProperties.hasProperty( s ) )
		convertFromString<int>(sendMessageTo, gProperties.getProperty( s ), std::dec);
    else
    {
        std::cerr << "[CRITICAL] Physical object #" << _id << " (switch) missing sendMessageTo value (integer, >0).\n";
        exit(-1);
    }
}

void SwitchObject::step()
{
    stepPhysicalObject();
}

void SwitchObject::isTouched( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (switch) touched by robot #" << __idAgent << std::endl;
}

void SwitchObject::isWalked( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (switch) walked upon by robot #" << __idAgent << std::endl;

    if ( (size_t)sendMessageTo < gPhysicalObjects.size() )
        gPhysicalObjects[sendMessageTo]->isWalked(-1);

    unregisterObject();
    hide();
    _visible = false;

    regrow = true;
    regrowTime = regrowTimeMax;
}
