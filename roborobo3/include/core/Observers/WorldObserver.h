/*
 *  WorldObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */



#ifndef WORLDOBSERVER_H
#define WORLDOBSERVER_H

#include "Observers/Observer.h"

class World;

class WorldObserver : public Observer
{
	//private:
	protected:
		World *_world;
		
	public:
		//Initializes the variables
		WorldObserver( World *__world );
		virtual ~WorldObserver();
				
		virtual void reset();
		virtual void step();
		
};

#endif

