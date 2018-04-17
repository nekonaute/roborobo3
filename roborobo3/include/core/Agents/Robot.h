/*
 *  Robot.h
 *  roborobo
 *
 *  Created by Nicolas on 17/12/08.
 *  Copyright 2008. All rights reserved.
 *
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Agents/Agent.h"

class World;
class Controller;
class AgentObserver;
class RobotWorldModel;
class WorldObserver;

class Robot : public Agent
{
	
	private:
	
		Controller *_controller;
		AgentObserver *_agentObserver;
	
		RobotWorldModel *_wm;

		// real displacement (x/y format, delta applied to real coordinates (_xReal,_yReal))
		double _xDeltaReal;
		double _yDeltaReal;
	
		double _lastAgentAbsoluteOrientation; // backup in case of collision

		int _iterations;
    
        void traceRayRGB(SDL_Surface * dst, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b); // for screen rendering only
        int castSensorRay(SDL_Surface * image, double x1, double y1, double * x2pt, double * y2pt , int __maxValue = -1);
    
        void initRobotPhysics( );
        void applyRobotPhysics( );

	public:
		Robot( World *__world );
		virtual ~Robot();

		void reset() override;

		void stepBehavior() override;
		void stepBehavior(const Uint8* __keyboardStates) override;

		void callObserver(); // call the embedded genetic engine -- always called before stepBehavior.

		void applyDynamics(); // use transl/rot delta to compute x/y delta.

		//Moves the agent
		void move( int __recursiveIt = 0 );
		
		void show(SDL_Surface *surface = gScreen) override;

		bool isCollision();

		void setCoordReal (int __x, int __y); // agent is centered on point
		
		void registerRobot();
		void unregisterRobot();
		
		void behaviorUpdate_remoteController(const Uint8* __keyStates);

        Controller* getController() { return _controller; }
        AgentObserver* getObserver() { return _agentObserver; }
		WorldObserver* getWorldObserver();
        RobotWorldModel* getWorldModel() { return _wm; }
    
        virtual std::string inspect( std::string prefix = "" );
    
		void displayInfo(); // display input/output values on console. (used for inspect agent mode, cf. help on console) -- keep it simple: one line only.

		
		// return a list of agents in the vincinity (ie. in the communication area)
		// the content of this list depends on the max communication range (see properties file).
		// works only if communication network is active.
		/* std::vector<int> getNeighbors(); */
};



#endif /* ROBOTAGENT_H */


