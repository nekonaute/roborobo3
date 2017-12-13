/*
 *  Controller.h
 *  roborobo-online
 *
 *  Created by Nicolas on 19/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>

class RobotWorldModel;

class Controller
{
	protected:
		RobotWorldModel *_wm;

	public:
		Controller( );
		Controller( RobotWorldModel *__wm );
        virtual ~Controller();
				
		virtual void reset() = 0;
		virtual void step() = 0;
    
        RobotWorldModel* getWorldModel() { return _wm; }
    
        virtual std::string inspect( std::string prefix = "" );
};


#endif

