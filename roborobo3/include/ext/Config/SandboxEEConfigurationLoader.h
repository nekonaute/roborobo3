/*
 * MedeaConfigurationLoader.h
 */

#ifndef SANDBOXEECONFIGURATIONLOADER_H
#define SANDBOXEECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class SandboxEEConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		SandboxEEConfigurationLoader();
		~SandboxEEConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
