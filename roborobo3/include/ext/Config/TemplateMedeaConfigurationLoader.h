/*
 * MedeaConfigurationLoader.h
 */

#ifndef TEMPLATEMEDEACONFIGURATIONLOADER_H
#define TEMPLATEMEDEACONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class TemplateMedeaConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		TemplateMedeaConfigurationLoader();
		~TemplateMedeaConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
