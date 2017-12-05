/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "World/CircleObject.h"
#include "RoboroboMain/roborobo.h"
#include <Utilities/Graphics.h>
#include "Utilities/Misc.h"

CircleObject::CircleObject( int __id ) : PhysicalObject( __id ) // a unique and consistent __id should be given as argument
{
	std::string s = "";
	std::stringstream out;
	out << getId();
    
    s = "physicalObject[";
	s += out.str();
	s += "].radius";
	if ( gProperties.hasProperty( s ) )
		convertFromString<double>(_radius, gProperties.getProperty( s ), std::dec);
	else
    {
        if ( gVerbose )
            std::cerr << "[MISSING] Physical Object #" << _id << " (of super type CircleObject) missing radius (integer, >=0). Check default value.\n";
        gProperties.checkAndGetPropertyValue("gPhysicalObjectDefaultRadius", &_radius, true);
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].footprintRadius";
	if ( gProperties.hasProperty( s ) )
		convertFromString<double>(_footprintRadius, gProperties.getProperty( s ), std::dec);
	else
    {
        if ( gVerbose )
            std::cerr << "[MISSING] Physical Object #" << _id << " (of super type CircleObject) missing footprint radius (integer, >=0). Check default value.\n";
        gProperties.checkAndGetPropertyValue("gPhysicalObjectDefaultFootprintRadius", &_footprintRadius, true);
    }

    double x = 0.0, y = 0.0;
    x = getXCenterPixel();
	y = getYCenterPixel();
    
    int tries = 0;
    bool randomLocation = false;

    if ( x == -1.0 || y == -1.0 )
    {
        tries = tries + findRandomLocation();
        randomLocation = true;
    }
    else
    {
        if ( canRegister() == false )  // i.e. user-defined location, but cannot register. Pick random.
        {
            std::cerr << "[CRITICAL] cannot use user-defined initial location (" << x << "," << y << ") for physical object #" << getId() << " (localization failed). EXITING.";
            exit(-1);
        }
        randomLocation = false;
    }
    
    if ( gVerbose )
    {
        std::cout << "[INFO] Physical Object #" << getId() << " (of super type CircleObject) positioned at ( "<< std::setw(5) << getXCenterPixel() << " , " << std::setw(5) << getYCenterPixel() << " ) -- ";
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
    
    /*
    if ( _visible )
    {
        registerObject();
    }
    registered = true;
    /**/
    /* BP.2017-12-05 !n */
    if ( canRegister() ) // in case location is not possible (may occur if findRandomLocation() failed, and gLocationFinderExitOnFail is false)
    {
        if ( _visible )
        {
            registerObject();
        }
        registered = true;
    }
    else
    {
        _visible = false;
        registered = false;
    }
    /**/
}


void CircleObject::show(SDL_Surface *surface) // display on screen (called in the step() method if gDisplayMode=0 and _visible=true)
{
    //  draw footprint
    
    Uint8 r = 0xF0;
    Uint8 g = 0xF0;
    Uint8 b = 0xF0;
    Uint32 color = SDL_MapRGBA(surface->format,r,g,b,SDL_ALPHA_OPAQUE);
    
    for (Sint16 xColor = getXCenterPixel() - Sint16(_footprintRadius) ; xColor < getXCenterPixel() + Sint16(_footprintRadius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_footprintRadius) ; yColor < getYCenterPixel() + Sint16 (_footprintRadius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _footprintRadius)
            {
                putPixel32_secured(surface, xColor, yColor,  color);
            }
        }
    }
    
    // draw object
    
    color = SDL_MapRGBA(surface->format,_displayColorRed,_displayColorGreen,_displayColorBlue,SDL_ALPHA_OPAQUE);
    
	for (Sint16 xColor = getXCenterPixel() - Sint16(_radius) ; xColor < getXCenterPixel() + Sint16(_radius) ; xColor++)
	{
		for (Sint16 yColor = getYCenterPixel() - Sint16(_radius) ; yColor < getYCenterPixel() + Sint16 (_radius); yColor ++)
		{
			if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _radius)
			{
                putPixel32_secured(surface, xColor, yColor,  color);
			}
		}
	}
}

void CircleObject::hide()
{
    //  hide footprint (draw white)
    
    Uint8 r = 0xFF;
    Uint8 g = 0xFF;
    Uint8 b = 0xFF;
    
    Uint32 color = SDL_MapRGBA(gScreen->format,r,g,b,SDL_ALPHA_OPAQUE);
    
    for (Sint16 xColor = getXCenterPixel() - Sint16(_footprintRadius) ; xColor < getXCenterPixel() + Sint16(_footprintRadius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_footprintRadius) ; yColor < getYCenterPixel() + Sint16 (_footprintRadius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _footprintRadius)
            {
                putPixel32_secured(gScreen, xColor, yColor,  color);
            }
        }
    }
    
    // hide object (draw white)
    
	for (Sint16 xColor = getXCenterPixel() - Sint16(_radius) ; xColor < getXCenterPixel() + Sint16(_radius) ; xColor++)
	{
		for (Sint16 yColor = getYCenterPixel() - Sint16(_radius) ; yColor < getYCenterPixel() + Sint16 (_radius); yColor ++)
		{
			if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _radius)
			{
                putPixel32_secured(gScreen, xColor, yColor,  color);
			}
		}
	}
}

bool CircleObject::canRegister()
{
    // test shape
    for (Sint16 xColor = getXCenterPixel() - Sint16(_radius) ; xColor < getXCenterPixel() + Sint16(_radius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_radius) ; yColor < getYCenterPixel() + Sint16 (_radius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _radius)
            {
                Uint32 pixel = getPixel32_secured( gEnvironmentImage, xColor, yColor);
                if ( pixel != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
                    return false; // collision!
            }
        }
    }
    
    //  test footprint (pixels from both ground image and environment image must be empty)
    for (Sint16 xColor = getXCenterPixel() - Sint16(_footprintRadius) ; xColor < getXCenterPixel() + Sint16(_footprintRadius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_footprintRadius) ; yColor < getYCenterPixel() + Sint16 (_footprintRadius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _footprintRadius)
            {
                Uint32 pixelFootprint = getPixel32_secured( gFootprintImage, xColor, yColor);
                Uint32 pixelEnvironment = getPixel32_secured( gEnvironmentImage, xColor, yColor);
                if (
                        pixelEnvironment != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ||
                        ( gFootprintImage_restoreOriginal == true  && pixelFootprint != getPixel32_secured( gFootprintImageBackup, xColor, yColor ) ) || // case: ground as initialized or rewritten (i.e. white)
                        ( gFootprintImage_restoreOriginal == false && pixelFootprint != SDL_MapRGBA( gFootprintImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ) // case: only white ground
                   )
                    return false; // collision!
            }
        }
    }
    
    return true;
}

void CircleObject::registerObject()
{
    int id_converted = _id + gPhysicalObjectIndexStartOffset;
    
    //  draw footprint
    
    Uint32 color = SDL_MapRGBA( gFootprintImage->format, (Uint8)((id_converted & 0xFF0000)>>16), (Uint8)((id_converted & 0xFF00)>>8), (Uint8)(id_converted & 0xFF), SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = getXCenterPixel() - Sint16(_footprintRadius) ; xColor < getXCenterPixel() + Sint16(_footprintRadius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_footprintRadius) ; yColor < getYCenterPixel() + Sint16 (_footprintRadius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _footprintRadius)
            {
                putPixel32_secured(gFootprintImage, xColor, yColor,  color);
            }
        }
    }
    
    // draw object
    
    color = SDL_MapRGBA( gEnvironmentImage->format, (Uint8)((id_converted & 0xFF0000)>>16), (Uint8)((id_converted & 0xFF00)>>8), (Uint8)(id_converted & 0xFF), SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = getXCenterPixel() - Sint16(_radius) ; xColor < getXCenterPixel() + Sint16(_radius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_radius) ; yColor < getYCenterPixel() + Sint16 (_radius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _radius)
            {
                putPixel32_secured(gEnvironmentImage, xColor, yColor,  color);//color);
            }
        }
    }
}

void CircleObject::unregisterObject()
{
    //  clear footprint
    
    Uint32 color = SDL_MapRGBA( gFootprintImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = getXCenterPixel() - Sint16(_footprintRadius) ; xColor < getXCenterPixel() + Sint16(_footprintRadius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_footprintRadius) ; yColor < getYCenterPixel() + Sint16 (_footprintRadius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _footprintRadius)
            {
                if ( gFootprintImage_restoreOriginal == true )
                {
                    color = getPixel32_secured( gFootprintImageBackup, xColor, yColor);
                    putPixel32_secured(gFootprintImage, xColor, yColor,  color);
                }
                else
                    putPixel32_secured(gFootprintImage, xColor, yColor,  color);
            }
        }
    }
    
    // clear object
    
    color = SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = getXCenterPixel() - Sint16(_radius) ; xColor < getXCenterPixel() + Sint16(_radius) ; xColor++)
    {
        for (Sint16 yColor = getYCenterPixel() - Sint16(_radius) ; yColor < getYCenterPixel() + Sint16 (_radius); yColor ++)
        {
            if ((sqrt ( pow (xColor-getXCenterPixel(),2) + pow (yColor - getYCenterPixel(),2))) < _radius)
            {
                putPixel32_secured(gEnvironmentImage, xColor, yColor,  color);//color);
            }
        }
    }
}
