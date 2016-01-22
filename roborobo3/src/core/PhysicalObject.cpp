#include "World/PhysicalObject.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "World/World.h"

#include <iomanip>

PhysicalObject::PhysicalObject( int __id ) // a unique and consistent __id should be given as argument
{
    _id = __id;
    init();
}

void PhysicalObject::init()
{
    double x = 0.0, y = 0.0;
    
	std::string s = "";
	std::stringstream out;
	out << getId();
    
    s = "physicalObject[";
	s += out.str();
	s += "].regrowTimeMax";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<int>(regrowTimeMax, gProperties.getProperty( s ), std::dec);
	}
    else
    {
        regrowTimeMax = gPhysicalObjectDefaultRegrowTimeMax;
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].overwrite";
	if ( gProperties.hasProperty( s ) )
        gProperties.checkAndGetPropertyValue(s,&overwrite,true);
    else
    {
        overwrite = gPhysicalObjectDefaultOverwrite;
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].relocate";
	if ( gProperties.hasProperty( s ) )
        gProperties.checkAndGetPropertyValue(s,&relocate,true);
    else
    {
        relocate = gPhysicalObjectDefaultRelocate;
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].visible";
	if ( gProperties.hasProperty( s ) )
        gProperties.checkAndGetPropertyValue(s,&_visible,true);
	else
        gProperties.checkAndGetPropertyValue("gPhysicalObjectsVisible", &_visible, true);
    
    Uint32 colorValue;
    
    s = "physicalObject[";
	s += out.str();
	s += "].displayColorRed";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<Uint32>(colorValue, gProperties.getProperty( s ), std::dec);
        _displayColorRed = (colorValue & 0x000000FF);
	}
    else
    {
        _displayColorRed = gPhysicalObjectDefaultDisplayColorRed; // default
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].displayColorGreen";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<Uint32>(colorValue, gProperties.getProperty( s ), std::dec);
        _displayColorGreen = (colorValue & 0x000000FF);
	}
    else
    {
        _displayColorGreen = gPhysicalObjectDefaultDisplayColorGreen; // default
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].displayColorBlue";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<Uint32>(colorValue, gProperties.getProperty( s ), std::dec);
        _displayColorBlue = (colorValue & 0x000000FF);
	}
    else
    {
        _displayColorBlue = gPhysicalObjectDefaultDisplayColorBlue; // default
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].x";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<double>(x, gProperties.getProperty( s ), std::dec);
	}
	else
	{
        x = -1.0;
	}
    
	s = "physicalObject[";
	s += out.str();
	s += "].y";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<double>(y, gProperties.getProperty( s ), std::dec);
	}
	else
	{
        y = -1.0;
	}
    
    _position = Point2d(x,y);
    
	_xCenterPixel = x;
	_yCenterPixel = y;

}

int PhysicalObject::findRandomLocation( )
{
    double x = 0.0, y = 0.0;
    
    int tries = 0;
    
    do {
        x = (rand() % (gAreaWidth-20)) + 10;
        y = (rand() % (gAreaHeight-20)) + 10;
        
        _position = Point2d(x,y);
        
        _xCenterPixel = x;
        _yCenterPixel = y;
        
        tries++;
    } while ( canRegister() == false && tries < gLocationFinderMaxNbOfTrials );
    
    
    if ( tries == gLocationFinderMaxNbOfTrials )
    {
        std::cerr << "[CRITICAL] Random initialization of initial position for physical object #" << getId() << " after trying " << gLocationFinderMaxNbOfTrials << " random picks (all failed). There may be too few (none?) possible locations (you may try to manually set initial positions). EXITING.\n";
        exit(-1);
    }
    
    return tries;
}

void PhysicalObject::stepPhysicalObject()
{
    if ( regrow == true && regrowTime != -1 )
    {
        if ( regrowTime > 0 )
        {
            regrowTime--;
        }
        else
        {
            if ( relocate == true )
            {
                findRandomLocation(); // exit on fail.
                _visible = true;
                regrow = false;
                registerObject();
                if ( gDisplayMode <= 1 )
                    display();
            }
            else
            {
                if ( canRegister() == true || overwrite == true )
                {
                    _visible = true;
                    regrow = false;
                    registerObject();
                    if ( gDisplayMode <= 1 )
                        display();
                }
            }
        }
    }
    else
    {
        if ( _visible )
        {
            if ( gPhysicalObjectsRedraw == true )
            {
                registerObject();
            }
            if ( gRefreshUserDisplay )
                display();
        }
    }
}

bool PhysicalObject::isInstanceOf ( int index ) // static
{
    if ( index >= gPhysicalObjectIndexStartOffset && index <  gPhysicalObjectIndexStartOffset + (int)gPhysicalObjects.size() )
        return true;
    else
        return false;
}

