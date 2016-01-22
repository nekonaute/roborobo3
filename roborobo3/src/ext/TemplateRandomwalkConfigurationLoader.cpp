#if defined PRJ_TEMPLATERANDOMWALK || !defined MODULAR

#include "Config/TemplateRandomwalkConfigurationLoader.h"

#include "TemplateRandomwalk/include/TemplateRandomwalkWorldObserver.h"
#include "TemplateRandomwalk/include/TemplateRandomwalkAgentObserver.h"
#include "TemplateRandomwalk/include/TemplateRandomwalkController.h"

#include "WorldModels/RobotWorldModel.h"


TemplateRandomwalkConfigurationLoader::TemplateRandomwalkConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
}

TemplateRandomwalkConfigurationLoader::~TemplateRandomwalkConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TemplateRandomwalkConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TemplateRandomwalkWorldObserver(wm);
}

RobotWorldModel* TemplateRandomwalkConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TemplateRandomwalkConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TemplateRandomwalkAgentObserver(wm);
}

Controller* TemplateRandomwalkConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TemplateRandomwalkController(wm);
}


#endif
