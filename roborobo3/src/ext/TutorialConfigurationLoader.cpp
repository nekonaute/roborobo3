#if defined PRJ_TUTORIAL || !defined MODULAR

#include "Config/TutorialConfigurationLoader.h"
#include "Tutorial/include/TutorialWorldObserver.h"
#include "Tutorial/include/TutorialAgentObserver.h"
#include "Tutorial/include/TutorialController.h"
#include "WorldModels/RobotWorldModel.h"


TutorialConfigurationLoader::TutorialConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
}

TutorialConfigurationLoader::~TutorialConfigurationLoader()
{
	//nothing to do
}

WorldObserver* TutorialConfigurationLoader::make_WorldObserver(World* wm)
{
	return new TutorialWorldObserver(wm);
}

RobotWorldModel* TutorialConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* TutorialConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new TutorialAgentObserver(wm);
}

Controller* TutorialConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new TutorialController(wm);
}


#endif
