/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */



#ifndef TEMPLATEWANDERCONTROLLER_H
#define TEMPLATEWANDERCONTROLLER_H

#include "Controllers/Controller.h"
#include "RoboroboMain/common.h"

class RobotWorldModel;

class TemplateWanderController : public Controller
{
	public:
		TemplateWanderController( RobotWorldModel *__wm );
		~TemplateWanderController();
		
		void reset();
		void step();

};


#endif

