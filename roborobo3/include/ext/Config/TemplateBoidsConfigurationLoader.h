/*
 * MedeaConfigurationLoader.h
 */

#ifndef TEMPLATEBOIDSCONFIGURATIONLOADER_H
#define TEMPLATEBOIDSCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class TemplateBoidsConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		TemplateBoidsConfigurationLoader();
		~TemplateBoidsConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
