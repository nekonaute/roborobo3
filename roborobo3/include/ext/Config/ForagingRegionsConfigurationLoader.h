/*
 * ForagingRegionsConfigurationLoader.h
 */

#ifndef FORAGINGREGIONSCONFIGURATIONLOADER_H
#define FORAGINGREGIONSCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class ForagingRegionsConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		ForagingRegionsConfigurationLoader();
		~ForagingRegionsConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm) ;
};



#endif
