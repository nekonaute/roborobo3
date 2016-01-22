#if defined PRJ_TEMPLATEBOIDS || !defined MODULAR

#include "Config/TemplateBoidsConfigurationLoader.h"

#include "TemplateBoids/include/TemplateBoidsWorldObserver.h"
#include "TemplateBoids/include/TemplateBoidsAgentObserver.h"
#include "TemplateBoids/include/TemplateBoidsController.h"

#include "WorldModels/RobotWorldModel.h"

TemplateBoidsConfigurationLoader::TemplateBoidsConfigurationLoader()
{
}

TemplateBoidsConfigurationLoader::~TemplateBoidsConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TemplateBoidsConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TemplateBoidsWorldObserver(wm);
}

RobotWorldModel* TemplateBoidsConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TemplateBoidsConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TemplateBoidsAgentObserver(wm);
}

Controller* TemplateBoidsConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TemplateBoidsController(wm);
}

#endif
