/*
 *  AgentObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 24/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

 
#ifndef AGENTOBSERVER_H
#define AGENTOBSERVER_H 

#include "Observers/Observer.h"

class RobotWorldModel;

class AgentObserver : public Observer
{
	protected:
		RobotWorldModel *_wm;
		
	public:
		//Initializes the variables
		AgentObserver( );
		AgentObserver( RobotWorldModel *__wm );
		~AgentObserver();
				
		void reset();
		void step();
		
};


#endif

