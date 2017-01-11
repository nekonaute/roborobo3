/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef SANDBOXEEAGENTOBSERVER_H
#define SANDBOXEEAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/AgentObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "SandboxEE/include/SandboxEESharedData.h"

#include <iomanip>

class SandboxEEAgentObserver : public AgentObserver
{
	public:
		SandboxEEAgentObserver(RobotWorldModel *wm);
		~SandboxEEAgentObserver();

		void reset();
		void step();

};

#endif

