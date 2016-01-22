/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */



#ifndef TEMPLATEWANDERCONTROLLER_H
#define TEMPLATEWANDERCONTROLLER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Controllers/Controller.h"

#include "WorldModels/RobotWorldModel.h"

class TemplateWanderController : public Controller
{
	public:
		//Initializes the variables
		TemplateWanderController( RobotWorldModel *__wm );
		~TemplateWanderController();
		
		void reset();
		void step();
    
        void monitorSensory();
};


#endif

