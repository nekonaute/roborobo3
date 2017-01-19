/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef TEMPLATEMEDEAAGENTOBSERVER_H
#define TEMPLATEMEDEAAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/AgentObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateMedea/include/TemplateMedeaSharedData.h"
#include "TemplateEE/include/TemplateEEAgentObserver.h"

#include <iomanip>

class TemplateMedeaAgentObserver : public TemplateEEAgentObserver
{
	public:
        TemplateMedeaAgentObserver( RobotWorldModel *wm );
		~TemplateMedeaAgentObserver();

        virtual void step();
};

#endif

