/*
 *  Timer.h
 *  roborobo
 *
 *  Created by Nicolas on 16/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef TIMER_H
#define TIMER_H

#include "RoboroboMain/common.h"

//The timer
class Timer
{
    private:
		//The clock time when the timer started
		int startTicks;
    
		//The ticks stored when the timer was paused
		int pausedTicks;
    
		//The timer status
		bool paused;
		bool started;
    
	public:
		//Initializes variables
		Timer();
		~Timer(){};
    
		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();
    
		//Gets the timer's time
		int get_ticks();
		
		//Checks the status of the timer
		bool is_started();
		bool is_paused();    
};


#endif

