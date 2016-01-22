/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */



#ifndef TEMPLATERANDOMWALKCONTROLLER_H
#define TEMPLATERANDOMWALKCONTROLLER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Controllers/Controller.h"

#include "WorldModels/RobotWorldModel.h"

#include "Agents/Agent.h"
#include "Agents/Robot.h"

class TemplateRandomwalkController : public Controller
{
	public:
		//Initializes the variables
		TemplateRandomwalkController( RobotWorldModel *__wm );
		~TemplateRandomwalkController();
    
        std::vector<double> _params;
		
		void reset();
		void step();
    
        void monitorSensoryInformation();
};


#endif

