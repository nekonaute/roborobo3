/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef TEMPLATEVANILLAEEAGENTOBSERVER_H
#define TEMPLATEVANILLAEEAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/AgentObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateVanillaEE/include/TemplateVanillaEESharedData.h"

#include <iomanip>

class TemplateVanillaEEAgentObserver : public AgentObserver
{
	public:
		TemplateVanillaEEAgentObserver(RobotWorldModel *wm);
		~TemplateVanillaEEAgentObserver();

		void reset();
		void step();

};

#endif

