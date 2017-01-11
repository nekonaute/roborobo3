#if defined PRJ_SANDBOXEE || !defined MODULAR

#include "Config/SandboxEEConfigurationLoader.h"

#include "SandboxEE/include/SandboxEEWorldObserver.h"
#include "SandboxEE/include/SandboxEEAgentObserver.h"
#include "SandboxEE/include/SandboxEEController.h"

#include "WorldModels/RobotWorldModel.h"

SandboxEEConfigurationLoader::SandboxEEConfigurationLoader()
{
}

SandboxEEConfigurationLoader::~SandboxEEConfigurationLoader()
{
	//nothing to do
}

WorldObserver* SandboxEEConfigurationLoader::make_WorldObserver(World* wm)
{
	return new SandboxEEWorldObserver(wm);
}

RobotWorldModel* SandboxEEConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* SandboxEEConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new SandboxEEAgentObserver(wm);
}

Controller* SandboxEEConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new SandboxEEController(wm);
}

#endif
