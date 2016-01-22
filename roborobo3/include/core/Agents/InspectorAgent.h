/*
 *  InspectorAgent.h
 *  roborobo
 *
 *  Created by Nicolas on 17/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef INSPECTORAGENT_H
#define INSPECTORAGENT_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Agents/Agent.h"

//The dot
class InspectorAgent : public Agent
{
	private:

	public:
	
		//Initializes the variables
		InspectorAgent();
		virtual ~InspectorAgent() {};
		//InspectorAgent(SDL_Surface *__mask);	// NOT IMPLEMENTED
		
		// controller step
		virtual void stepBehavior();
		virtual void stepBehavior(const Uint8* __keyboardStates);
    
		void reset();
		
		//Moves the dot
		void move();

		
		//Shows the dot on the screen
		void show();
		
		/*
		//Sets the camera over the dot
		void set_camera();
	
		void getCoord(int &coord_x, int &coord_y)
		{
			coord_x=x;
			coord_y=y;
		}
	
		void setCoord (int __x, int __y) // agent is centered on point
		{
			x = __x - gDotWidth/2 ; 
			y = __y - gDotHeight/2 ;
		}
		*/
};



#endif /* INSPECTORAGENT_H */

