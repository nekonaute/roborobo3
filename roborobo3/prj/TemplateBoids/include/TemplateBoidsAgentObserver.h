/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 * NNlib: Leo Cazenille <leo.cazenille@upmc.fr>
 */


#ifndef TEMPLATEBOIDSAGENTOBSERVER_H
#define TEMPLATEBOIDSAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/AgentObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateBoids/include/TemplateBoidsSharedData.h"

#include <iomanip>

class TemplateBoidsAgentObserver : public AgentObserver
{
	public:
		TemplateBoidsAgentObserver(RobotWorldModel *wm);
		~TemplateBoidsAgentObserver();

		void reset();
		void step();

};

#endif

