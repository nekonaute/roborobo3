/*
 *  Controller.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 19/03/09.
 *
 */

#include "Controllers/Controller.h"

Controller::Controller(  )
{
	// nothing to do.
}


Controller::Controller( RobotWorldModel *__wm )
{
	_wm = __wm;
}

Controller::~Controller()
{
	// nothing to do.
}

std::string Controller::inspect( )
{
    // nothing to do.
    return "Controller::inspect() not implemented.\n";
}
