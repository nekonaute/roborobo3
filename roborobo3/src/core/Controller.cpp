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

void Controller::inspect( const std::string& prefix )
{
    // nothing to do.
    std::cout << prefix << "Controller::inspect() not implemented." << std::endl;
}
