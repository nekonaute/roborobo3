#include "World/SquareObject.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "World/World.h"

#include <iomanip>

SquareObject::SquareObject( int __id ) : PhysicalObject( __id ) // a unique and consistent __id should be given as argument
{
	std::string s = "";
	std::stringstream out;
	out << getId();
    
    s = "physicalObject[";
	s += out.str();
	s += "].solid_w";
	if ( gProperties.hasProperty( s ) )
		convertFromString<int>(_solid_w, gProperties.getProperty( s ), std::dec);
	else
    {
        if ( gVerbose )
            std::cerr << "[MISSING] Physical Object #" << _id << " (of super type SquareObject) missing solid_w (integer, >=0). Get default value.\n";
        gProperties.checkAndGetPropertyValue("gPhysicalObjectDefaultSolid_w", &_solid_w, true);
    }

    s = "physicalObject[";
	s += out.str();
	s += "].solid_h";
	if ( gProperties.hasProperty( s ) )
		convertFromString<int>(_solid_h, gProperties.getProperty( s ), std::dec);
	else
    {
        if ( gVerbose )
            std::cerr << "[MISSING] Physical Object #" << _id << " (of super type SquareObject) missing solid_h (integer, >=0). Get default value.\n";
        gProperties.checkAndGetPropertyValue("gPhysicalObjectDefaultSolid_h", &_solid_h, true);
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].soft_w";
	if ( gProperties.hasProperty( s ) )
		convertFromString<int>(_soft_w, gProperties.getProperty( s ), std::dec);
	else
    {
        if ( gVerbose )
            std::cerr << "[MISSING] Physical Object #" << _id << " (of super type SquareObject) missing soft_w (integer, >=0). Get default value.\n";
        gProperties.checkAndGetPropertyValue("gPhysicalObjectDefaultSoft_w", &_soft_h, true);
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].soft_h";
	if ( gProperties.hasProperty( s ) )
		convertFromString<int>(_soft_h, gProperties.getProperty( s ), std::dec);
	else
    {
        if ( gVerbose )
            std::cerr << "[MISSING] Physical Object #" << _id << " (of super type SquareObject) missing soft_h (integer, >=0). Get default value.\n";
        gProperties.checkAndGetPropertyValue("gPhysicalObjectDefaultSoft_h", &_soft_h, true);
    }

    double x = 0.0, y = 0.0;
    x = _xCenterPixel;
	y = _yCenterPixel;
    
    int tries = 0;
    bool randomLocation = false;
    bool userdefinedlocation = false;
    
    if ( x == -1.0 || y == -1.0 )
    {
        x = (rand() % (gAreaWidth-20)) + 10;
        y = (rand() % (gAreaHeight-20)) + 10;
        _xCenterPixel = x;
        _yCenterPixel = y;
        randomLocation = true;
        tries++;
    }
    else
        userdefinedlocation = true;
    
    if ( canRegister() == false )
    {
        if ( userdefinedlocation == true )
        {
            std::cerr << "[CRITICAL] cannot use user-defined initial location (" << x << "," << y << ") for physical object #" << getId() << " (localization failed). EXITING.";
            exit(-1);
        }
        else
        {
            tries = tries + findRandomLocation();
            randomLocation = true;
        }
    }
    
    if ( gVerbose )
    {
        std::cout << "[INFO] Physical Object #" << getId() << "  (of super type SquareObject) positioned at ( "<< std::setw(5) << _xCenterPixel << " , " << std::setw(5) << _yCenterPixel << " ) -- ";
        if ( randomLocation == false )
            std::cout << "[user-defined position]\n";
        else
        {
            std::cout << "[random pick after " << tries;
            if ( tries <= 1 )
                std::cout << " try]";
            else
                std::cout << " tries]";
            std::cout << "\n";
        }
    }
    
    if ( _visible )
    {
        registerObject();
        if ( gDisplayMode <= 1 )
            display();
    }
    
    regrow = false;
}

void SquareObject::display() // display on screen (called in the step() method if gDisplayMode=0 and _visible=true)
{
    //  draw footprint
    
    Uint8 r = 0xF0;
    Uint8 g = 0xF0;
    Uint8 b = 0xF0;

    Uint32 color = SDL_MapRGBA(gBackgroundImage->format,r,g,b,SDL_ALPHA_OPAQUE);
    
    for (Sint16 xColor = _xCenterPixel - Sint16(_soft_w/2) ; xColor < _xCenterPixel + Sint16(_soft_w/2) ; xColor++)
    {
        for (Sint16 yColor = _yCenterPixel - Sint16(_soft_h/2) ; yColor < _yCenterPixel + Sint16 (_soft_h/2); yColor ++)
        {
            putPixel32_secured(gBackgroundImage, xColor, yColor,  color);
        }
    }
    
    // draw object
    
    color = SDL_MapRGBA(gBackgroundImage->format,_displayColorRed,_displayColorGreen,_displayColorBlue,SDL_ALPHA_OPAQUE);
    
	for (Sint16 xColor = _xCenterPixel - Sint16(_solid_w/2) ; xColor < _xCenterPixel + Sint16(_solid_w/2) ; xColor++)
	{
		for (Sint16 yColor = _yCenterPixel - Sint16(_solid_h/2) ; yColor < _yCenterPixel + Sint16 (_solid_h/2); yColor ++)
		{
            putPixel32(gBackgroundImage, xColor, yColor,  color);
		}
	}
}

void SquareObject::hide()
{
    //  hide footprint (draw white)
    
    Uint8 r = 0xFF;
    Uint8 g = 0xFF;
    Uint8 b = 0xFF;
    
    Uint32 color = SDL_MapRGBA(gBackgroundImage->format,r,g,b,SDL_ALPHA_OPAQUE);
    
    for (Sint16 xColor = _xCenterPixel - Sint16(_soft_w/2) ; xColor < _xCenterPixel + Sint16(_soft_w/2) ; xColor++)
    {
        for (Sint16 yColor = _yCenterPixel - Sint16(_soft_h/2) ; yColor < _yCenterPixel + Sint16 (_soft_h/2); yColor ++)
        {
            putPixel32_secured(gBackgroundImage, xColor, yColor,  color);
        }
    }
    
    // hide object (draw white)
    
	for (Sint16 xColor = _xCenterPixel - Sint16(_solid_w/2) ; xColor < _xCenterPixel + Sint16(_solid_w/2) ; xColor++)
	{
		for (Sint16 yColor = _yCenterPixel - Sint16(_solid_h/2) ; yColor < _yCenterPixel + Sint16 (_solid_h/2); yColor ++)
		{
            putPixel32(gBackgroundImage, xColor, yColor,  color);
		}
	}
}

bool SquareObject::canRegister()
{
    // test shape
	for (Sint16 xColor = _xCenterPixel - Sint16(_solid_w/2) ; xColor < _xCenterPixel + Sint16(_solid_w/2) ; xColor++)
	{
		for (Sint16 yColor = _yCenterPixel - Sint16(_solid_h/2) ; yColor < _yCenterPixel + Sint16 (_solid_h/2); yColor ++)
        {
            Uint32 pixel = getPixel32_secured( gEnvironmentImage, xColor, yColor);
            if ( pixel != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
                return false; // collision!
        }
    }
    
    //  test footprint (pixels from both ground image and environment image must be empty)
    for (Sint16 xColor = _xCenterPixel - Sint16(_soft_w/2) ; xColor < _xCenterPixel + Sint16(_soft_w/2) ; xColor++)
    {
        for (Sint16 yColor = _yCenterPixel - Sint16(_soft_h/2) ; yColor < _yCenterPixel + Sint16 (_soft_h/2); yColor ++)
        {
            Uint32 pixel = getPixel32_secured( gGroundSensorImage, xColor, yColor);
            Uint32 pixel2 = getPixel32_secured( gEnvironmentImage, xColor, yColor);
            if ( pixel != SDL_MapRGBA( gGroundSensorImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) || pixel2 != SDL_MapRGBA( gGroundSensorImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
                return false; // collision!
        }
    }
    
    return true;
}

void SquareObject::registerObject()
{
    int id_converted = _id + gPhysicalObjectIndexStartOffset;
    
    //  draw footprint
    
    Uint32 color = SDL_MapRGBA( gGroundSensorImage->format, (Uint8)((id_converted & 0xFF0000)>>16), (Uint8)((id_converted & 0xFF00)>>8), (Uint8)(id_converted & 0xFF), SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = _xCenterPixel - Sint16(_soft_w/2) ; xColor < _xCenterPixel + Sint16(_soft_w/2) ; xColor++)
    {
        for (Sint16 yColor = _yCenterPixel - Sint16(_soft_h/2) ; yColor < _yCenterPixel + Sint16 (_soft_h/2); yColor ++)
        {
            putPixel32_secured(gGroundSensorImage, xColor, yColor,  color);
        }
    }
    
    // draw object
    
    color = SDL_MapRGBA( gEnvironmentImage->format, (Uint8)((id_converted & 0xFF0000)>>16), (Uint8)((id_converted & 0xFF00)>>8), (Uint8)(id_converted & 0xFF), SDL_ALPHA_OPAQUE );
    
	for (Sint16 xColor = _xCenterPixel - Sint16(_solid_w/2) ; xColor < _xCenterPixel + Sint16(_solid_w/2) ; xColor++)
	{
		for (Sint16 yColor = _yCenterPixel - Sint16(_solid_h/2) ; yColor < _yCenterPixel + Sint16 (_solid_h/2); yColor ++)
        {
            putPixel32_secured(gEnvironmentImage, xColor, yColor,  color);//color);
        }
    }
}

void SquareObject::unregisterObject()
{
    //  draw footprint
    
    Uint32 color = SDL_MapRGBA( gGroundSensorImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = _xCenterPixel - Sint16(_soft_w/2) ; xColor < _xCenterPixel + Sint16(_soft_w/2) ; xColor++)
    {
        for (Sint16 yColor = _yCenterPixel - Sint16(_soft_h/2) ; yColor < _yCenterPixel + Sint16 (_soft_h/2); yColor ++)
        {
            putPixel32_secured(gGroundSensorImage, xColor, yColor,  color);
        }
    }
    
    // draw object
    
    color = SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE );
    
	for (Sint16 xColor = _xCenterPixel - Sint16(_solid_w/2) ; xColor < _xCenterPixel + Sint16(_solid_w/2) ; xColor++)
	{
		for (Sint16 yColor = _yCenterPixel - Sint16(_solid_h/2) ; yColor < _yCenterPixel + Sint16 (_solid_h/2); yColor ++)
        {
            putPixel32_secured(gEnvironmentImage, xColor, yColor,  color);//color);
        }
    }
}