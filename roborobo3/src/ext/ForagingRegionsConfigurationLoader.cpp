#if defined PRJ_FORAGINGREGIONS || !defined MODULAR

#include "Config/ForagingRegionsConfigurationLoader.h"
#include "ForagingRegions/include/ForagingRegionsWorldObserver.h"
#include "ForagingRegions/include/ForagingRegionsAgentObserver.h"
#include "ForagingRegions/include/ForagingRegionsController.h"
#include "WorldModels/RobotWorldModel.h"

ForagingRegionsConfigurationLoader::ForagingRegionsConfigurationLoader()
{
}

ForagingRegionsConfigurationLoader::~ForagingRegionsConfigurationLoader()
{
	//nothing to do
}

WorldObserver* ForagingRegionsConfigurationLoader::make_WorldObserver(World* wm)
{
	return new ForagingRegionsWorldObserver(wm);
}

RobotWorldModel* ForagingRegionsConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* ForagingRegionsConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new ForagingRegionsAgentObserver(wm);
}

Controller* ForagingRegionsConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new ForagingRegionsController(wm);
}

#endif
