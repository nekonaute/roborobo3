/*
 *  WorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "Observers/WorldObserver.h"
#include "World/World.h"


WorldObserver::WorldObserver( World *__world ) 
{
	_world = __world;
}

WorldObserver::~WorldObserver()
{
	// nothing to do.
}

void WorldObserver::initPre() // called *before* first initializing Objects and Robots
{
	// nothing to do.
}

void WorldObserver::initPost() // called *after* first initializing Objects and Robots
{
    // nothing to do.
}

void WorldObserver::stepPre() // see header file for remark on stepPre/stepPost
{
	// nothing to do.
}

void WorldObserver::stepPost()
{
    // nothing to do.
}
