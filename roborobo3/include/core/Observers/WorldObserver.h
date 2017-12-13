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
		WorldObserver( World *__world );
		virtual ~WorldObserver();
				
    /* initPre() and initPost() methods
        these methods are called at the end of World::initWorld()
            initPre() is called before landmarks/objects/robots are set up according to the Properties file
            initPost() is called after landmarks/objects/robots are set up according to the Properties file, ie. just before simulation starts
        These methods are useful to log the state of the world before simulation starts (especially initPost()).
        These methods are also to change/initialize the simulation, e.g. setting up specifics (e.g. add/remove/move some objects, robots, landmarks)
     */
		virtual void initPre();
        virtual void initPost();
    
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

