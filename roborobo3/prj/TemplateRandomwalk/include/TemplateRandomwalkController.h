/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */



#ifndef TEMPLATERANDOMWALKCONTROLLER_H
#define TEMPLATERANDOMWALKCONTROLLER_H

#include "Controllers/Controller.h"

class RobotWorldModel;

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

