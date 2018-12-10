/*
 * TemplateHITEEConfigurationLoader.h
 */

#ifndef TEMPLATEHITEECONFIGURATIONLOADER_H
#define TEMPLATEHITEECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class TemplateHITEEConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		TemplateHITEEConfigurationLoader();
		~TemplateHITEEConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
