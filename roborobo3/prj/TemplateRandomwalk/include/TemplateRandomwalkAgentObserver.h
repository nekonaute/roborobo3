/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */
 
#ifndef TEMPLATERANDOMWALKAGENTOBSERVER_H
#define TEMPLATERANDOMWALKAGENTOBSERVER_H 

#include "Observers/AgentObserver.h"

class RobotWorldModel;

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

