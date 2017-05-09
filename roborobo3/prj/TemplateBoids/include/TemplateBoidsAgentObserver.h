/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef TEMPLATEBOIDSAGENTOBSERVER_H
#define TEMPLATEBOIDSAGENTOBSERVER_H

#include "Observers/AgentObserver.h"

class RobotWorldModel;

class TemplateBoidsAgentObserver : public AgentObserver
{
	public:
		TemplateBoidsAgentObserver(RobotWorldModel *wm);
		~TemplateBoidsAgentObserver();

		void reset();
		void step();

};

#endif

