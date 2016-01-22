/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 * NNlib: Leo Cazenille <leo.cazenille@upmc.fr>
 */


#ifndef TEMPLATEMEDEAAGENTOBSERVER_H
#define TEMPLATEMEDEAAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/AgentObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateMedea/include/TemplateMedeaSharedData.h"

#include <iomanip>

class TemplateMedeaAgentObserver : public AgentObserver
{
	public:
		TemplateMedeaAgentObserver(RobotWorldModel *wm);
		~TemplateMedeaAgentObserver();

		void reset();
		void step();

};

#endif

