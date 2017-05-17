/*
 *  PassiveObject.h
 *  roborobo
 *
 *  Created by Nicolas on 29/4/2011. Revised 4/1/2014.
 *
 */

#ifndef LANDMARKOBJECT_H
#define LANDMARKOBJECT_H

#include "RoboroboMain/common.h"
#include "Utilities/Geometry.h"

extern SDL_Surface *gScreen;

class LandmarkObject
{
    private :
    
    int _id;
    static int _nextId;
    
    Uint8 r = 0xFF;
    Uint8 g = 0xD4;
    Uint8 b = 0x82;
    
    // coordinates
    Sint16 _xCenterPixel;
    Sint16 _yCenterPixel;

    protected :
    
    double _radius; // radius, in pixels.
    bool _visible; // display option (default: true)
    
    int _state; // a state value, not used. Could be used for any particular purpose.
    
    public :
    
    LandmarkObject();
    ~LandmarkObject();
    
    int getId()
    {
        return _id;
    }
    
    void setCoordinates(int x, int y)
    {
        hide(); // hide previous position (if any)
        
        _xCenterPixel = x;
        _yCenterPixel = y;
    }
    
    Point2d getCoordinates()
    {
        return Point2d(_xCenterPixel,_yCenterPixel);
    }
    
    int getX()
    {
        return _xCenterPixel;
    }

    int getY()
    {
        return _yCenterPixel;
    }
    
    void setColor( Uint8 _r, Uint8 _g, Uint8 _b )
    {
        r = _r;
        g = _g;
        b = _b;
    }
    
    int getState() { return _state; }
    void setState( int state ) { _state = state; }
    
    bool isVisible() { return _visible; }
    
    virtual void step();
    
    virtual void show(SDL_Surface *surface = gScreen);
    virtual void hide();
    
};

#endif
