#if defined PRJ_TEMPLATEWANDER || !defined MODULAR

#include "Config/TemplateWanderConfigurationLoader.h"

#include "TemplateWander/include/TemplateWanderWorldObserver.h"
#include "TemplateWander/include/TemplateWanderAgentObserver.h"
#include "TemplateWander/include/TemplateWanderController.h"

#include "WorldModels/RobotWorldModel.h"


TemplateWanderConfigurationLoader::TemplateWanderConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
}

TemplateWanderConfigurationLoader::~TemplateWanderConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TemplateWanderConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TemplateWanderWorldObserver(wm);
}

RobotWorldModel* TemplateWanderConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TemplateWanderConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TemplateWanderAgentObserver(wm);
}

Controller* TemplateWanderConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TemplateWanderController(wm);
}


#endif
