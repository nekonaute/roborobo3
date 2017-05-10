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
#include "TemplateEE/include/TemplateEEAgentObserver.h"

class TemplateVanillaEEAgentObserver : public TemplateEEAgentObserver
{
	public:
		TemplateVanillaEEAgentObserver(RobotWorldModel *wm);
		~TemplateVanillaEEAgentObserver();
    
        virtual void step();
};

#endif

