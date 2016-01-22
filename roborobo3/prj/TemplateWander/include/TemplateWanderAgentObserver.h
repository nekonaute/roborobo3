/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */
 
#ifndef TEMPLATEWANDERAGENTOBSERVER_H
#define TEMPLATEWANDERAGENTOBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"
#include "Observers/AgentObserver.h"

class TemplateWanderAgentObserver : public AgentObserver
{
	public:
		TemplateWanderAgentObserver( );
		TemplateWanderAgentObserver( RobotWorldModel *__wm );
		~TemplateWanderAgentObserver();
				
		void reset();
		void step();
		
};


#endif

