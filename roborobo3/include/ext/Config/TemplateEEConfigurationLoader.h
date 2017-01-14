/*
 * TemplateEEConfigurationLoader.h
 */

#ifndef TEMPLATEEECONFIGURATIONLOADER_H
#define TEMPLATEEECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class TemplateEEConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		TemplateEEConfigurationLoader();
		~TemplateEEConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
