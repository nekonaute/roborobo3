#if defined PRJ_TEMPLATEHITEE || !defined MODULAR

#include "Config/TemplateHITEEConfigurationLoader.h"
#include "TemplateHITEE/include/TemplateHITEEWorldObserver.h"
#include "TemplateHITEE/include/TemplateHITEEAgentObserver.h"
#include "TemplateHITEE/include/TemplateHITEEController.h"
#include "WorldModels/RobotWorldModel.h"

TemplateHITEEConfigurationLoader::TemplateHITEEConfigurationLoader()
{
}

TemplateHITEEConfigurationLoader::~TemplateHITEEConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TemplateHITEEConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TemplateHITEEWorldObserver(wm);
}

RobotWorldModel* TemplateHITEEConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TemplateHITEEConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TemplateHITEEAgentObserver(wm);
}

Controller* TemplateHITEEConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TemplateHITEEController(wm);
}

#endif
