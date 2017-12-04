/*
 * EEcreditassignementConfigurationLoader.h
 */

#ifndef EECREDITASSIGNEMENTCONFIGURATIONLOADER_H
#define EECREDITASSIGNEMENTCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class EEcreditassignementConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		EEcreditassignementConfigurationLoader();
		~EEcreditassignementConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
