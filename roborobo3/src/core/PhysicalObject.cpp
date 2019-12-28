/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "World/PhysicalObject.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "World/World.h"

PhysicalObject::PhysicalObject( int __id ) // a unique and consistent __id should be given as argument
{
    _id = __id;

    this->lastRelocationDate = gWorld->getIterations();
    //std::cout << "[DEBUG]" << lastRelocationDate << "\n";
    
    init();
}

void PhysicalObject::init()
{
    double x = 0.0, y = 0.0;
    
	std::string s = "";
	std::stringstream out;
	out << getId();
    
    regrowTime = 0;
    
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
        gProperties.checkAndGetPropertyValue(s,&relocateObject,true);
    else
    {
        relocateObject = gPhysicalObjectDefaultRelocate;
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
    
    setCoordinates( x, y );
}

int PhysicalObject::findRandomLocation( )
{   /**/
    // Original method, prior to 29/11/2019
    double x = 0.0, y = 0.0;
    
    int tries = 0;
    
    do {
        x = ( randint() % ( gPhysicalObjectsInitAreaWidth  ) ) + gPhysicalObjectsInitAreaX;
        y = ( randint() % ( gPhysicalObjectsInitAreaHeight ) ) + gPhysicalObjectsInitAreaY;
        
        //x = (randint() % (gAreaWidth-20)) + 10;  // deprecated
        //y = (randint() % (gAreaHeight-20)) + 10; // deprecated
        
        setCoordinates( x, y );
        
        tries++;
    } while ( canRegister() == false && tries < gLocationFinderMaxNbOfTrials );

    if ( tries == gLocationFinderMaxNbOfTrials )
    {
        if ( gExpensiveObjectLocationFindingMethod == true )
        {
            /**/
            // New method 2019-11-29 -- more efficient, but horribly costly wrt computation. Test *all* possible positions, pick a random one. Horrible as in O(n^2) with n = length of arena. n^2 (or rather length*width) is not only the worst case, but also the default number of operations, as all available positions must be identified to ensure unbiased random sampling of one position over all available. Use with care as it will significantly slow down your code for large and dense arena.
            // enable/disable in config file using gExpensiveObjectLocationFindingMethod property
            
            std::cout << "[INFO] FindRandomLocation, Trying *computationally expensive* method (gExpensiveObjectLocationFindingMethod is True).\n";
            
            tries = 0;
            int nbLocations = 0;
            
            std::vector<std::pair<int,int>> locations;
            
            for ( int x = gPhysicalObjectsInitAreaX ; x < gPhysicalObjectsInitAreaWidth ; x++ )
                for ( int y = gPhysicalObjectsInitAreaY ; y < gPhysicalObjectsInitAreaHeight ; y++ )
                {
                    setCoordinates( x, y );
                    if ( canRegister() == true )
                    {
                        locations.push_back(std::make_pair(x,y));
                        nbLocations++;
                    }
                }
            
            if ( locations.empty() == false )
            {
                int randomIndex = (int)(locations.size() * random01());
                std::pair<int,int> selectedLocation = locations.at(randomIndex);
                setCoordinates(selectedLocation.first, selectedLocation.second);
                tries = gLocationFinderMaxNbOfTrials + nbLocations;
            }
            else
            {
                tries = gLocationFinderMaxNbOfTrials; // Failed.
            }
            
            std::cout << "[INFO] FindRandomLocation: picked one location out of " << nbLocations << " possible locations.\n";
            /**/
        }
    }
    
    if ( tries == gLocationFinderMaxNbOfTrials )
    {
        if ( gLocationFinderExitOnFail == true )
        {
            if ( gExpensiveObjectLocationFindingMethod == false )
                std::cerr << "[CRITICAL] Random initialization of initial position for physical object #" << getId() << " after trying " << gLocationFinderMaxNbOfTrials << " random picks (all failed). There may be too few (none?) possible locations (you may try to manually set initial positions, or setting gExpensiveObjectLocationFindingMethod to true in the configuration file). EXITING.\n";
            else
                std::cerr << "[CRITICAL] Random initialization of initial position for physical object #" << getId() << " after trying *all* possible locations (all failed). There are no possible location. EXITING.\n";
            exit(-1);
        }
        else
        {
            std::cerr << "[WARNING] Random initialization of initial position for physical object #" << getId() << " after trying ";
            if ( gExpensiveObjectLocationFindingMethod == false )
                std::cerr << gLocationFinderMaxNbOfTrials << " random picks (all failed). Retry later.\n";
            else
                std::cerr << "all current possible locations (all failed). Retry later.\n";
            regrowTime = 1;
            setCoordinates( -1, -1 );
        }
    }
    
    return tries;
}

void PhysicalObject::stepPhysicalObject()
{
    if ( registered == false && regrowTime != -1 )
    {
        if ( regrowTime > 0 )
        {
            regrowTime--;
        }
        else
        {
            if ( relocateObject == true )
            {
                findRandomLocation(); // fail: exit or return (x,y)=(-1,-1)
                if ( getXReal() != -1 ) // check if new location is possible
                {
                    //std::cout << "REGROW!\n";
                    _visible = true;
                    registered = true;
                    registerObject();
                }
            }
            else
            {
                if ( canRegister() == true || overwrite == true )
                {
                    _visible = true;
                    registered = true;
                    registerObject();
                }
            }
        }
    }
    else
    {
        if ( getXReal() != -1 ) // check if (new) location is possible
            if ( _visible )
            {
                if ( gPhysicalObjectsRedraw == true )
                {
                    registerObject();
                }
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


/*
 Given an object not registered in the environment, tries to re-register at once.
 Return false (and do nothing) if not possible at desired location.
 Exit if function is called when it should not -- calling this function can be critical, including registering multiple instances of the same object, thus breaking consistency.
*/
bool PhysicalObject::triggerRegrow()
{
    if ( registered == true )  // exit if object is already registered in the environment
    {
        std::cerr << "[CRITICAL] physical object #" << getId() << ": attempt to call triggerRegrow() method while object already exists in the environment. EXITING.\n";
        exit(-1);
    }
    
    if ( canRegister() == false ) // check is position is free.
    {
        return false;
    }
    else
    {
        regrowTime = gPhysicalObjectDefaultRegrowTimeMax;
        _visible = true;
        registered = true;
        registerObject();
    }
    
    return true;
}

std::string PhysicalObject::inspect( std::string prefix )
{
    return std::string(prefix + "PhysicalObject::inspect() not implemented.\n");
}

void PhysicalObject::relocate()
{
    if ( getXCenterPixel() != -1.0 && getYCenterPixel() != -1.0 ) // if registered
        unregisterObject();
    
    findRandomLocation();
    
    if ( getXCenterPixel() != -1.0 && getYCenterPixel() != -1.0 ) // not registered
    {
        registerObject();
    }
}

bool PhysicalObject::relocate( int x, int y )
{
    int backup_x = getXCenterPixel();
    int backup_y = getYCenterPixel();
    
    setCoordinates( x,y );
    
    if ( canRegister() == true )
    {
        if ( getXCenterPixel() != -1.0 && getYCenterPixel() != -1.0 ) // if registered
            unregisterObject();
        registerObject();
        return true;
    }
    else
    {
        setCoordinates(backup_x, backup_y);
        return false;
    }
}

void PhysicalObject::registerObject()
{
    //std::cout << "[DEBUG][0]" << lastRelocationDate << "\n";
    this->lastRelocationDate = gWorld->getIterations();
    //std::cout << "[DEBUG][1]" << lastRelocationDate << "\n";
}

int PhysicalObject::getTimestepSinceRelocation()
{
    int a = gWorld->getIterations();
    int b = this->lastRelocationDate;
    return ( a - b );
}
