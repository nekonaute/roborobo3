#if defined PRJ_SANDBOX || !defined MODULAR

#include "Config/SandboxConfigurationLoader.h"
#include "Sandbox/include/SandboxWorldObserver.h"
#include "Sandbox/include/SandboxAgentObserver.h"
#include "Sandbox/include/SandboxController.h"
#include "WorldModels/RobotWorldModel.h"

SandboxConfigurationLoader::SandboxConfigurationLoader()
{
}

SandboxConfigurationLoader::~SandboxConfigurationLoader()
{
	//nothing to do
}

WorldObserver* SandboxConfigurationLoader::make_WorldObserver(World* wm)
{
	return new SandboxWorldObserver(wm);
}

RobotWorldModel* SandboxConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* SandboxConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new SandboxAgentObserver(wm);
}

Controller* SandboxConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new SandboxController(wm);
}

#endif
