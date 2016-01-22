#include "World/LandmarkObject.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"

#include "World/World.h"

#include <iomanip>

int LandmarkObject::_nextId = 0;

LandmarkObject::LandmarkObject()
{
	_id = LandmarkObject::_nextId; // set unique id.
	LandmarkObject::_nextId++; // update id reference.

	_visible=true;

    bool randomLocation = false;
    
    double x = 0.0, y = 0.0;
    
	std::string s = "";
	s += "landmark[";
	std::stringstream out;
	out << getId();
	s += out.str();
	s += "].x";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<double>(x, gProperties.getProperty( s ), std::dec);
	}
	else
	{
		x = (rand() % (gAreaWidth-20)) + 10;
        randomLocation = true;
	}
    
	s = "landmark[";
	s += out.str();
	s += "].y";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<double>(y, gProperties.getProperty( s ), std::dec);
	}
	else
	{
		y = (rand() % (gAreaHeight-20)) + 10;
        randomLocation = true;	}
    
	_position = Point2d(x,y);
    
	_xCenterPixel = x;
	_yCenterPixel = y;
    
	gProperties.checkAndGetPropertyValue("VisibleLandmarks", &_visible, false);
	if ( _visible)
	{
		display();
	}
    
	gProperties.checkAndGetPropertyValue("gLandmarkRadius", &_radius, true);
    
    if ( gVerbose )
        std::cout << "[INFO] Landmark #" << getId() << " positioned at ( "<< std::setw(5) << x << " , " << std::setw(5) << y << " ) -- ";
    if ( randomLocation == false )
    {
        if ( gVerbose )
            std::cout << "[user-defined position]\n";
    }
    else
    {
        if ( gVerbose )
            std::cout << "[random pick]\n";
    }
}

LandmarkObject::~LandmarkObject()
{
	// called if deleted from the list of inanimatedObject
}

void LandmarkObject::step()
{
	if ( _visible && gDisplayMode != 2)
	{
		display();
	}
}

void LandmarkObject::display() // display on screen (called in the step() method if gDisplayMode=0 and _visible=true)
{
    Uint32 color = SDL_MapRGBA(gBackgroundImage->format,r,g,b,SDL_ALPHA_OPAQUE);
   
    
	for (Sint16 xColor = _xCenterPixel - Sint16(_radius) ; xColor < _xCenterPixel + Sint16(_radius) ; xColor++)
	{
		for (Sint16 yColor = _yCenterPixel - Sint16(_radius) ; yColor < _yCenterPixel + Sint16 (_radius); yColor ++)
		{
			if ((sqrt ( pow (xColor-_xCenterPixel,2) + pow (yColor - _yCenterPixel,2))) < _radius)
			{
                putPixel32(gBackgroundImage, xColor, yColor,  color);//color);
			}
		}
	}
}

void LandmarkObject::hide()
{
    Uint8 r = 0xFF;
    Uint8 g = 0xFF;
    Uint8 b = 0xFF;
    Uint32 color = SDL_MapRGBA(gBackgroundImage->format,r,g,b,SDL_ALPHA_OPAQUE);

    
	for (Sint16 xColor = _xCenterPixel - Sint16(_radius) ; xColor < _xCenterPixel + Sint16(_radius) ; xColor++)
	{
		for (Sint16 yColor = _yCenterPixel - Sint16(_radius) ; yColor < _yCenterPixel + Sint16 (_radius); yColor ++)
		{
			if ((sqrt ( pow (xColor-_xCenterPixel,2) + pow (yColor - _yCenterPixel,2))) < _radius)
			{
				putPixel32(gBackgroundImage, xColor, yColor, color);
			}
		}
	}
}

