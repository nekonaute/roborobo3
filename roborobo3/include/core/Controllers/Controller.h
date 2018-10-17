/*
 *  Controller.h
 *  roborobo-online
 *
 *  Created by Nicolas on 19/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include "RoboroboMain/common.h"
#include "Utilities/Geometry.h"

class RobotWorldModel;

class Controller
{
    private:
    
        int lastRefreshIteration;
        bool checkRefresh();
        void refreshInputs();

	protected:
    
		RobotWorldModel *_wm;

        std::vector<double> distanceSensors;
        std::vector<int> objectDetectors;
        std::vector<int> objectTypeDetectors;
        std::vector<int> robotDetectors;
        std::vector<int> robotGroupDetector;
        std::vector<double> robotRelativeOrientationDetectors;
        std::vector<int> wallDetectors;
    
        double redGroundDetectors;
        double greenGroundDetectors;
        double blueGroundDetectors;

        double landmark_closest_DistanceDetector;
        double landmark_closest_DirectionDetector;

        // remark: other landmarks can be accessed directly from _wm->updateLandmarkSensor(id_landmark).

	public:
    
		Controller( );
		Controller( RobotWorldModel *__wm );
        virtual ~Controller();
				
		virtual void reset() = 0;
		virtual void step() = 0;
    
        RobotWorldModel* getWorldModel() { return _wm; }
    
        virtual std::string inspect( std::string prefix = "" );
    
        // #### #### #### #### #### #### #### #### #### #### #### #### ####
        //
        // Accessing methods
        //
        // The following methods can be used to access all information
        // available to the robot. These are wrappers for more complex
        // internal functions and should be used to write your projects.
        //
        // #### #### #### #### #### #### #### #### #### #### #### #### ####

        // returns normalized distance in [0,1]
        double getDistanceAt( int sensorId )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return distanceSensors[sensorId];
        }

        // returns 0 (no object) or 1 (object)
        int getObjectAt( int sensorId );

        // returns target object's type (relevant if target object exists)
        // returns an integer. Check PhysicalObjectFactory class for existing types.
        // most common types are: 0 (round), 1 (energy item), 2 (gate), 3 (switch), 4 (movable), ...?
        int getObjectTypeAt( int sensorId );
    
        // returns robot's id, or -1 (not a robot)
        int getRobotIdAt( int sensorId );
    
        // returns target robot's group, or -1
        int getRobotGroupIdAt( int sensorId );

        // returns 0 (not a wall) or 1 (wall)
        int getWallAt( int sensorId );
    
        // returns target robot's orientation wrt this robot's facing direction (relevant if target robot exists), mapped to [-1,+1]
        // i.e. if both robots roughly go in the same direction, it returns a value around 0. If opposite directions, value returned is close to -1 or +1  
        double getRobotRelativeOrientationAt( int sensorId );
    
        // return a value in [0,1] (red component)
        double getRedGroundDetector( );
    
        // return a value in [0,1] (green component)
        double getGreenGroundDetector( );

        // return a value in [0,1] (blue component)
        double getBlueGroundDetector( );
    
        // return distance to closest landmark (pixel-based euclidian distance - normalized in [0,1] -- using maximum possible distance in arena
        double getClosestLandmarkDistance()
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return landmark_closest_DistanceDetector;
        }

        // return angle to closest landmark - mapped to [-1,+1]
        double getClosestLandmarkOrientation()
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return landmark_closest_DirectionDetector;
        }
    
        // get actual absolute orientation (wrt. east / rightwards -- Clock-wise) -- mapped to [-1,+1]
        double getOrientation();
    
        // get actual linear speed
        double getLinearSpeed();
    
        // get x/y coordinate in the arena
        Point2d getPosition();

        // setTranslation(value) takes a value in [-1,+1]
        void setTranslation( double value );
    
        // setRotation(value) takes a value in [-1,+1]
        void setRotation( double value );
    
        // get this robot Id (anything from 0 to gInitialNumberOfRobots)
        int getId();
    
        // get this robot "true" actual translational speed (ie. actual measured translation per timestep, not desired -- the two may differ)
        double getActualTranslation();
    
        // get this robot "true" actual rotational speed (ie. actual measured rotation per timestep, not desired -- the two may differ)
        double getActualRotation();
    
        // robot's orientation w.r.t. North (ie. upwards), mapped in [-1,+1]
        double getCompass();
    
        // #### #### #### #### #### #### #### #### #### #### #### #### ####
    
};


#endif
