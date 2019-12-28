/*
 * SandboxConfigurationLoader.h
 */

#ifndef SANDBOXCONFIGURATIONLOADER_H
#define SANDBOXCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class SandboxConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		SandboxConfigurationLoader();
		~SandboxConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
