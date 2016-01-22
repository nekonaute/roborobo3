/*
 * TemplateRandomwalkConfigurationLoader.h
 */

#ifndef TEMPLATERANDOMWALKCONFIGURATIONLOADER_H
#define	TEMPLATERANDOMWALKCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class TemplateRandomwalkConfigurationLoader : public ConfigurationLoader
{
	public:
		TemplateRandomwalkConfigurationLoader();
		~TemplateRandomwalkConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};

#endif
