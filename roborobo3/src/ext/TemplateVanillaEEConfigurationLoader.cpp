#if defined PRJ_TEMPLATEVANILLAEE || !defined MODULAR

#include "Config/TemplateVanillaEEConfigurationLoader.h"

#include "TemplateVanillaEE/include/TemplateVanillaEEWorldObserver.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEAgentObserver.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEController.h"

#include "WorldModels/RobotWorldModel.h"

TemplateVanillaEEConfigurationLoader::TemplateVanillaEEConfigurationLoader()
{
}

TemplateVanillaEEConfigurationLoader::~TemplateVanillaEEConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TemplateVanillaEEConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TemplateVanillaEEWorldObserver(wm);
}

RobotWorldModel* TemplateVanillaEEConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TemplateVanillaEEConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TemplateVanillaEEAgentObserver(wm);
}

Controller* TemplateVanillaEEConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TemplateVanillaEEController(wm);
}

#endif
