/*
 *  RobotWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "WorldModels/RobotWorldModel.h"

#include "World/World.h"

RobotWorldModel::RobotWorldModel() : _cameraSensors ( boost::extents[12][7] )
{
    _initSensor = false;
    
    _xReal = 0.0;
    _yReal = 0.0;

    _agentAbsoluteOrientation = 0.0;
    _agentAbsoluteLinearSpeed = 0.0;

    _actualTranslationalValue = 0.0;
    _actualRotationalVelocity = 0.0;
    
	_robotLED_green = _robotLED_blue = 0; // default value (grey)
	_robotLED_red = 255;
    
    _energyRequestValue = 0.0; // default value -- if asked, dont request for energy.
    
    _energyLevel = gEnergyInit;
    
    _desiredTranslationalValue = 0;
    _desiredRotationalVelocity = 0;
}

RobotWorldModel::~RobotWorldModel()
{
}

void RobotWorldModel::initCameraSensors ( int nbSensors )
{
    if ( _initSensor == true )
    {
        std::cerr << "[ERROR] robot #" << this->getId() << " - tried to initialize sensors a second time.";
        exit(-1);
    }

    this->_cameraSensors.resize(boost::extents[nbSensors][7]);

    _initSensor = true;
}

void RobotWorldModel::updateLandmarkSensor( int __landmarkId )
{
    if ( (size_t)__landmarkId > gLandmarks.size() )
    {
        std::cout << "[ERROR] Landmark " << __landmarkId << " does not exist in RobotWorldModel::updateLandmarkSensor(landmarkId). Exiting." << std::endl;
        exit (-1);
    }
    
    Point2d posRobot(_xReal,_yReal);
    Point2d landmarkCoordinates = gLandmarks[__landmarkId].getPosition();
    
    double distanceToLandmark = getEuclidianDistance (posRobot,landmarkCoordinates);
    double diffAngleToLandmark = getAngleToTarget(posRobot,_agentAbsoluteOrientation,landmarkCoordinates);

    setLandmarkDirectionAngleValue( diffAngleToLandmark / 180.0 );
   
    //cast the shortest distance between 0 and 1
    if ( distanceToLandmark > gSensorRange )
        setLandmarkDistanceValue(1.0);
    else
        setLandmarkDistanceValue( distanceToLandmark / (double)gSensorRange );
    
    // monitoring
    if ( gVerbose && gDisplayMode <= 1 && gMonitorRobot && gRobotIndexFocus == getId() )
    {
        std::cout << "[ Target landmark " << __landmarkId << " : " << std::setw(8) << diffAngleToLandmark << "°, " <<  std::setw(10) << distanceToLandmark << "pix" << " ] --- [ ";
        std::cout << "Sensor values  : " << std::setw(4) << getLandmarkDirectionAngleValue() << " , " << std::setw(4) << getLandmarkDistanceValue() << " ] --- [ ";
        std::cout << "Battery-level  : " << std::setw(6) << getEnergyLevel() << " ] --- [";
        std::cout << "Energy Requested (if asked) : " << std::setw(4) << getEnergyRequestValue() << " ]" << std::endl;
    }

}

void RobotWorldModel::updateLandmarkSensor()
{
    double distanceToClosestLandmark = 0.0;
    
    // * find closest landmark and compute orientation to it
    
    double diffAngleToClosestLandmark;
    
    if ( gLandmarks.size() > 0 )
    {
        Point2d posRobot(_xReal,_yReal);
        Point2d closestPoint;

        distanceToClosestLandmark = getEuclidianDistance (posRobot,gLandmarks[0].getPosition());
        int iClosest = 0;
        
        for ( unsigned int i = 1 ; i < gLandmarks.size() ; i++ )
        {
            double distance = getEuclidianDistance (posRobot,gLandmarks[i].getPosition());
            
            if ( distance < distanceToClosestLandmark )
            {
                distanceToClosestLandmark = distance;
                iClosest = i;
            }
        }
        
        closestPoint = gLandmarks[iClosest].getPosition();
        diffAngleToClosestLandmark = getAngleToTarget(posRobot,_agentAbsoluteOrientation,closestPoint);
    }
    else
        diffAngleToClosestLandmark = 0;
    
    //cast the diffAngle between -1 and 1
    setLandmarkDirectionAngleValue( diffAngleToClosestLandmark / 180.0 );
    
    //cast the shortest distance between 0 and 1
    if ( distanceToClosestLandmark > gSensorRange )
        setLandmarkDistanceValue(1.0);
    else
        setLandmarkDistanceValue( distanceToClosestLandmark / (double)gSensorRange );

    // monitoring
    if ( gVerbose && gDisplayMode <= 1 && gMonitorRobot && gRobotIndexFocus == getId() )
    {
        std::cout << "[ Closest landmark : " << std::setw(8) << diffAngleToClosestLandmark << "°, " <<  std::setw(10) << distanceToClosestLandmark << "pix" << " ] --- [ ";
        std::cout << "Sensor values  : " << std::setw(4) << getLandmarkDirectionAngleValue() << " , " << std::setw(4) << getLandmarkDistanceValue() << " ] --- [ ";
        std::cout << "Battery-level  : " << std::setw(6) << getEnergyLevel() << " ] --- [";
        std::cout << "Energy Requested (if asked) : " << std::setw(4) << getEnergyRequestValue() << " ]" << std::endl;
    }
}
