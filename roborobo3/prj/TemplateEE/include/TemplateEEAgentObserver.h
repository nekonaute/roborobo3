/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef TEMPLATEEEAGENTOBSERVER_H
#define TEMPLATEEEAGENTOBSERVER_H

#include "Observers/AgentObserver.h"

class RobotWorldModel;

class TemplateEEAgentObserver : public AgentObserver
{
	public:
		TemplateEEAgentObserver(RobotWorldModel *wm);
		~TemplateEEAgentObserver();

		virtual void reset();
		virtual void step();

};

#endif

