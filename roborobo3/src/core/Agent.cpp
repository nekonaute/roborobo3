/*
 *  agent.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Agents/Agent.h"

Agent::Agent()
{
	// dont forget to call your own init() method
}


void Agent::getCoord(int &coord_x, int &coord_y)
{
    coord_x=_x;
    coord_y=_y;
}

void Agent::updateCoord(int &__xUpdate, int &__yUpdate)
{
    _x += __xUpdate;
    _y += __yUpdate;
}

void Agent::setCoord (int __x, int __y) // agent is centered on point
{
    _x = __x - gRobotWidth/2 ;
    _y = __y - gRobotHeight/2 ;
}

/**
    * Display agent on screen. Add information caption if needed.
	* (render mode only) 
    */
void Agent::show() // display on screen -- method may be overriden
{    
    //Show the dot
    apply_surface( _x - gCamera.x, _y - gCamera.y, gRobotDisplayImage, gScreen );
}



 /**
    * update the agent position in the environment. Apply simple physics (ie. obstacle collision detection and consequences).
    */
void Agent::move() // the interface btw agent and world -- in more complex envt, this should be handled by the "world".
{
/*
	// update position

	int x_old = _x; // backup old position in case of collision
	int y_old = _y;
	
    _x += _xDelta; //Move the dot left or right
    _y += _yDelta; //Move the dot up or down


	// * collision with (image) border of the environment - position at border, then bounce

    //If the dot went too far to the left or right
    if ( ( _x < 0 ) || ( _x + gRobotWidth > gAreaWidth ) )
	{
		_xDelta=-_xDelta/2; // bounce
        _x -= _xDelta; // move back
		_x= _x < 0 ? 0 : gAreaWidth-gRobotWidth;
	}
    
    //If the dot went too far up or down
    if ( ( _y < 0 ) || ( _y + gRobotHeight > gAreaHeight ) )
    {
        _yDelta=-_yDelta/2; // bounce
        _y -= _yDelta; // move back
		_y = _y < 0 ? 0 : gAreaHeight-gRobotHeight;
    }	
	
	// * Object/environment collision check 

	bool collision = false;

	// COLLISION DETECTION ROUTINE, Version used since 2012-06-06
    for ( int i = 0 ; i != gRobotMaskData.size() ; i++ )
    {
        Uint32 pixel = getPixel32( gForegroundImage , _x + gRobotMaskData[i][0] , _y + gRobotMaskData[i][1] );        
        
        if ( pixel != SDL_MapRGBA( gForegroundImage->format, 0xFF, 0xFF, 0xFF, 0 ) )
        {
			collision = true;
			break;
        }
    }
        
	
	if ( collision ) // bounce (TODO: position at collision point)
	{	
		_x=x_old;
		_y=y_old;
		_xDelta = -_xDelta/2;
		_yDelta = -_yDelta/2;
	}
 */
}


/* position camera */
void Agent::set_camera()
{
    //Center the camera over the dot
    gCamera.x = ( _x + gRobotWidth / 2 ) - gScreenWidth / 2;
    gCamera.y = ( _y + gRobotHeight / 2 ) - gScreenHeight / 2;
    
    //Keep the camera in bounds.
    if( gCamera.x < 0 )        gCamera.x = 0;    
    if( gCamera.y < 0 )        gCamera.y = 0;    
    if( gCamera.x > gAreaWidth  - gCamera.w )        gCamera.x = gAreaWidth - gCamera.w;    
    if( gCamera.y > gAreaHeight - gCamera.h )        gCamera.y = gAreaHeight - gCamera.h;    

}

bool Agent::isInstanceOf ( int index ) // static
{
    if ( index >= gRobotIndexStartOffset && index < gRobotIndexStartOffset+gNumberOfRobots )
        return true;
    else
        return false;
}





