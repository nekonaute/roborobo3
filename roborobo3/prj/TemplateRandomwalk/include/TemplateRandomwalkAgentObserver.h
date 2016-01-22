/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */
 
#ifndef TEMPLATERANDOMWALKAGENTOBSERVER_H
#define TEMPLATERANDOMWALKAGENTOBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"
#include "Observers/AgentObserver.h"

class TemplateRandomwalkAgentObserver : public AgentObserver
{
	public:
		TemplateRandomwalkAgentObserver( );
		TemplateRandomwalkAgentObserver( RobotWorldModel *__wm );
		~TemplateRandomwalkAgentObserver();
				
		void reset();
		void step();
		
};


#endif

