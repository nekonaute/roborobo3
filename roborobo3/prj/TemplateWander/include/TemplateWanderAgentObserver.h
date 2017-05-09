/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */
 
#ifndef TEMPLATEWANDERAGENTOBSERVER_H
#define TEMPLATEWANDERAGENTOBSERVER_H 

#include "Observers/AgentObserver.h"

class RobotWorldModel;

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

