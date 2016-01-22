/*
 *  Controller.h
 *  roborobo-online
 *
 *  Created by Nicolas on 19/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/Graphics.h"
#include "WorldModels/RobotWorldModel.h"

class Controller
{
	protected:
		RobotWorldModel *_wm;

	public:
		//Initializes the variables
		Controller( );
		Controller( RobotWorldModel *__wm );
        virtual ~Controller();
				
		virtual void reset() = 0;
		virtual void step() = 0;
    
        RobotWorldModel* getWorldModel() { return _wm; }
};


#endif

