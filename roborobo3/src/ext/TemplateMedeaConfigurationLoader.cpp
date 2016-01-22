#if defined PRJ_TEMPLATEMEDEA || !defined MODULAR

#include "Config/TemplateMedeaConfigurationLoader.h"

#include "TemplateMedea/include/TemplateMedeaWorldObserver.h"
#include "TemplateMedea/include/TemplateMedeaAgentObserver.h"
#include "TemplateMedea/include/TemplateMedeaController.h"

#include "WorldModels/RobotWorldModel.h"

TemplateMedeaConfigurationLoader::TemplateMedeaConfigurationLoader()
{
}

TemplateMedeaConfigurationLoader::~TemplateMedeaConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TemplateMedeaConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TemplateMedeaWorldObserver(wm);
}

RobotWorldModel* TemplateMedeaConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TemplateMedeaConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TemplateMedeaAgentObserver(wm);
}

Controller* TemplateMedeaConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TemplateMedeaController(wm);
}

#endif
