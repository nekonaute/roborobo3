/*
 * TutorialConfigurationLoader.h
 */

#ifndef TUTORIALCONFIGURATIONLOADER_H
#define	TUTORIALCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class TutorialConfigurationLoader : public ConfigurationLoader
{
	public:
		TutorialConfigurationLoader();
		~TutorialConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};

#endif
