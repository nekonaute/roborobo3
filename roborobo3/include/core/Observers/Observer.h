/*
 *  Observer.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */

 
#ifndef OBSERVER_H
#define OBSERVER_H 

class Observer
{
	public:
		Observer();
		virtual ~Observer();
	
		virtual void stepPre();
        virtual void stepPost();
};


#endif

