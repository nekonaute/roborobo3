#if defined PRJ_TEMPLATEEE || !defined MODULAR

#include "Config/TemplateEEConfigurationLoader.h"

#include "TemplateEE/include/TemplateEEWorldObserver.h"
#include "TemplateEE/include/TemplateEEAgentObserver.h"
#include "TemplateEE/include/TemplateEEController.h"

#include "WorldModels/RobotWorldModel.h"

TemplateEEConfigurationLoader::TemplateEEConfigurationLoader()
{
}

TemplateEEConfigurationLoader::~TemplateEEConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TemplateEEConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TemplateEEWorldObserver(wm);
}

RobotWorldModel* TemplateEEConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TemplateEEConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TemplateEEAgentObserver(wm);
}

Controller* TemplateEEConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TemplateEEController(wm);
}

#endif
