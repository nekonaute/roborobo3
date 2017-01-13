/*
 * TemplateVanillaEEConfigurationLoader.h
 */

#ifndef TEMPLATEVANILLAEECONFIGURATIONLOADER_H
#define TEMPLATEVANILLAEECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class TemplateVanillaEEConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		TemplateVanillaEEConfigurationLoader();
		~TemplateVanillaEEConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
