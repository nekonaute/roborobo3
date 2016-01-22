/*
 *  AgentObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 24/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Observers/AgentObserver.h"



AgentObserver::AgentObserver( )
{
}

AgentObserver::AgentObserver( RobotWorldModel *__wm )
{
	_wm = (RobotWorldModel*)__wm;
}

AgentObserver::~AgentObserver()
{
	// nothing to do.
}

void AgentObserver::reset()
{
	// nothing to do.
}

void AgentObserver::step()
{
	// nothing to do.
}

