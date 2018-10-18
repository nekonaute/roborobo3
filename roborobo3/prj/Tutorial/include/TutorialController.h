/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */



#ifndef TUTORIALCONTROLLER_H
#define TUTORIALCONTROLLER_H

#include "Controllers/Controller.h"
#include "RoboroboMain/common.h"

class RobotWorldModel;

class TutorialController : public Controller
{
	public:
		TutorialController( RobotWorldModel *__wm );
		~TutorialController();
    
		void reset();
		void step();
};


#endif

