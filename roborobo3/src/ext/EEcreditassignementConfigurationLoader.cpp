#if defined PRJ_EECREDITASSIGNEMENT || !defined MODULAR

#include "Config/EEcreditassignementConfigurationLoader.h"
#include "EEcreditassignement/include/EEcreditassignementWorldObserver.h"
#include "EEcreditassignement/include/EEcreditassignementAgentObserver.h"
#include "EEcreditassignement/include/EEcreditassignementController.h"
#include "WorldModels/RobotWorldModel.h"

EEcreditassignementConfigurationLoader::EEcreditassignementConfigurationLoader()
{
}

EEcreditassignementConfigurationLoader::~EEcreditassignementConfigurationLoader()
{
	//nothing to do
}

WorldObserver* EEcreditassignementConfigurationLoader::make_WorldObserver(World* wm)
{
	return new EEcreditassignementWorldObserver(wm);
}

RobotWorldModel* EEcreditassignementConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* EEcreditassignementConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new EEcreditassignementAgentObserver(wm);
}

Controller* EEcreditassignementConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new EEcreditassignementController(wm);
}

#endif
