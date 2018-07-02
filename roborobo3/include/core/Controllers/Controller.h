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
        std::vector<int> robotSamegroupDetector;
        std::vector<double> robotOrientationDetectors;
        std::vector<int> wallDetectors;
    
        double redGroundDetectors;
        double greenGroundDetectors;
        double blueGroundDetectors;

        double landmark_closest_DirectionDetector;
        double landmark_closest_OrientationDetector;

        // remark: other landmarks can be accessed directly from _wm->updateLandmarkSensor(id_landmark).

	public:
    
		Controller( );
		Controller( RobotWorldModel *__wm );
        virtual ~Controller();
				
		virtual void reset() = 0;
		virtual void step() = 0;
    
        RobotWorldModel* getWorldModel() { return _wm; }
    
        virtual std::string inspect( std::string prefix = "" );
    
        // accessing

        double getDistance( int i )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return distanceSensors[i];
        }

        int getObject( int i )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return objectDetectors[i];
        }

        int getObjectType( int i )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return objectTypeDetectors[i];
        }
    
        int getRobot( int i )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return robotDetectors[i];
        }
    
        int getRobotSamegroupDetector( int i )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return robotSamegroupDetector[i];
        }

        int getWall( int i )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return wallDetectors[i];
        }

        int getRobotOrientationDetectors( int i )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return robotOrientationDetectors[i];
        }
    
        double getRedGroundDetectors( )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return redGroundDetectors;
        }
    
        double getGreenGroundDetectors( )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return greenGroundDetectors;
        }
    
        double getBlueGroundDetectors( )
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return blueGroundDetectors;
        }
    
        double getClosestLandmarkDirection()
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return landmark_closest_DirectionDetector;
        }
    
        double getClosestLandmarkOrientation()
        {
            if ( checkRefresh() == false ) { refreshInputs(); }
            return landmark_closest_OrientationDetector;
        }
    
        void setTranslation( double value );
    
        void setRotation( double value );
    
        int getId();
    
        double getActualTranslation();
    
        double getActualRotation();
    
        // Agent orientation w.r.t. North (ie. upwards) in degrees.
        double getCompass();
};


#endif
