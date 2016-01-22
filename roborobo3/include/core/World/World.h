/*
 *  World.h
 *  roborobo
 *
 *  Created by Nicolas on 16/01/09.
 *
 */


#ifndef WORLD_H
#define WORLD_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Agents/Robot.h"

#include "Utilities/Misc.h"

#include "Observers/WorldObserver.h"

class Robot;

class World
{
	protected:

		std::vector<Robot*> robots;
		std::vector<bool> robotRegistry;
        //This array tracks agents which are registered (ie. drawn) in the foreground image. Note that in the current implementation, tracking registered agent could be remove (in practical, agents are always registered, except for the very brief moment when an agent moves (unregister=>move=>register). The historical reason for this was that in previous implementation some agents could avoid registering whenever agent-agent collision was known to be impossible. However, this method was later shown to provide no speed-up when many small agents are considered. Also, gain is marginal for smaller number of robots.

		int _iterations;

		//True if there is a variation in the number of agent
		bool _agentsVariation;
		
		WorldObserver *_worldObserver;

    public:

		World();
		~World();

		bool loadFiles();
		//bool loadProperties( std::string __propertiesFilename );

		void initWorld();
		void resetWorld();
		void updateWorld(const Uint8 *__keyboardStates = NULL);
		
		Robot* getRobot( int index );
		bool isRobotRegistered( int index );

		//delete an agent from the simulator. No other functions to call
        // THIS FUNCTION SHOULD NOT BE IMPLEMENTED AND SHOULD NEVER BE CALLED
        // Roborobo assumes that the 'agents' list indexes matches agent's id. Mandatory for CellularMap consistency.
		void deleteRobot (int index );
    
		//add an agent in the simulator. No other functions to call
		void addRobot(Robot *robot);
		
		void deleteLandmark(int __index );
		//void addLandmark(LandmarkObject* objectToAdd);
	
		int getIterations();
		WorldObserver* getWorldObserver();
		int getNbOfRobots();
};



#endif // WORLD_H

