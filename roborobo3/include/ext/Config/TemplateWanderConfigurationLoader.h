/*
 * TemplateWanderConfigurationLoader.h
 */

#ifndef TEMPLATEWANDERCONFIGURATIONLOADER_H
#define	TEMPLATEWANDERCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class TemplateWanderConfigurationLoader : public ConfigurationLoader
{
	public:
		TemplateWanderConfigurationLoader();
		~TemplateWanderConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};

#endif
