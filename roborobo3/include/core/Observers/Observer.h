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
		//Initializes the variables
		Observer();
		virtual ~Observer();
	
		virtual void reset();
		virtual void step();
		
};


#endif

