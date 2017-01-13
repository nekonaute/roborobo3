/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef TEMPLATEEEAGENTOBSERVER_H
#define TEMPLATEEEAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/AgentObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateEE/include/TemplateEESharedData.h"

#include <iomanip>

class TemplateEEAgentObserver : public AgentObserver
{
	public:
		TemplateEEAgentObserver(RobotWorldModel *wm);
		~TemplateEEAgentObserver();

		void reset();
		void step();

};

#endif

