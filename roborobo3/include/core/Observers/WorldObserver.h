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
				
    /* reset() is called at the end of World::initWorld(). This is where you may log the state of the world before simulation starts, or change some elements (e.g. add/remove/move some objects) */ 
		virtual void reset();
    
    /* a remark on stepPre() and stepPost():
     the update ordering for one step of roborobo is
     N_o*object.step() => worldObserver.stepPre() => N_a*agent.step() => N*a*agent.move() => worldObserver.stepPost()
     ie.
     to observe the state of the world or modify agent state before agents move, use stepPre()
     to observe the state of the world or modify agent state just after agents move, use stepPost()
     to modify objects, it might be better to do it in stepPost() (so that object can be updated afterwards)
     to modify objects before the simulation starts, use init()
     you may use init() or stepPre() to initialize agents before simulation starts
     init() is called once before any object is updated
     stepPre() is called the first time just before the first object's updating, and then at each iteration
     
     A final note on backward compatibility: from 2017-11-29, step() is renamed stepPre()
     */
		virtual void stepPre();
        virtual void stepPost();
		
};

#endif

