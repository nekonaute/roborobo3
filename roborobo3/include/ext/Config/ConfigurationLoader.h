/*
 *	ConfigurationLoader.h
 */

#ifndef CONFIGURATIONLOADER_H
#define CONFIGURATIONLOADER_H


#include "Observers/WorldObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "Observers/AgentObserver.h"
#include "Controllers/Controller.h"

#include "RoboroboMain/common.h"
#include "Utilities/ExtendedProperties.h"
#include <string.h>


class ConfigurationLoader
{
	protected:
		ConfigurationLoader();
		~ConfigurationLoader();
		
	public:

		static ConfigurationLoader* make_ConfigurationLoader(std::string configurationLoaderObjectName);

		virtual WorldObserver* make_WorldObserver(World* wm) = 0 ;
		virtual RobotWorldModel* make_RobotWorldModel() = 0 ;
		virtual AgentObserver* make_AgentObserver(RobotWorldModel* wm) = 0 ;
		virtual Controller* make_Controller(RobotWorldModel* wm) = 0;
};


#endif
