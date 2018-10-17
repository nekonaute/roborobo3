/*
 *  RobotWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "WorldModels/RobotWorldModel.h"
#include "RoboroboMain/roborobo.h"

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
    if ( __landmarkId > gNbOfLandmarks )
    {
        std::cout << "[ERROR] Landmark " << __landmarkId << " does not exist in RobotWorldModel::updateLandmarkSensor(landmarkId). Exiting." << std::endl;
        exit (-1);
    }
    
    Point2d posRobot(_xReal,_yReal);
    Point2d landmarkCoordinates = gLandmarks[__landmarkId]->getCoordinates();
    
    double distanceToLandmark = getEuclideanDistance (posRobot,landmarkCoordinates);
    double diffAngleToLandmark = getAngleToTarget(posRobot,_agentAbsoluteOrientation,landmarkCoordinates);
    
    // orientation wrt to landmark. mapped to [-1,+1]
    setLandmarkDirectionAngleValue( diffAngleToLandmark / 180.0 );
    
    // distance to landmark. normalized in [0,1] -- using maximum possible distance in arena
    setLandmarkDistanceValue( distanceToLandmark / (double)(gAreaWidth*gAreaHeight) );
    
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
    if ( gNbOfLandmarks == 0 )
    {
        std::cout << "[ERROR] There is no landmark to consider in RobotWorldModel::updateLandmarkSensor(). Exiting." << std::endl;
        exit (-1);
    }

    
    double distanceToClosestLandmark = 0.0;
    
    // * find closest landmark and compute orientation to it
    
    double diffAngleToClosestLandmark;
    
    if ( gNbOfLandmarks > 0 )
    {
        Point2d posRobot(_xReal,_yReal);
        Point2d closestPoint;

        distanceToClosestLandmark = getEuclideanDistance (posRobot,gLandmarks[0]->getCoordinates());
        int iClosest = 0;
        
        for ( int i = 1 ; i < gNbOfLandmarks ; i++ )
        {
            double distance = getEuclideanDistance (posRobot,gLandmarks[i]->getCoordinates());
            
            if ( distance < distanceToClosestLandmark )
            {
                distanceToClosestLandmark = distance;
                iClosest = i;
            }
        }
        
        closestPoint = gLandmarks[iClosest]->getCoordinates();
        diffAngleToClosestLandmark = getAngleToTarget(posRobot,_agentAbsoluteOrientation,closestPoint);
    }
    else
        diffAngleToClosestLandmark = 0;
    
    // orientation wrt to landmark. mapped to [-1,+1]
    setLandmarkDirectionAngleValue( diffAngleToClosestLandmark / 180.0 );
    
    // distance to landmark. normalized in [0,1] -- using maximum possible distance in arena
    setLandmarkDistanceValue( distanceToClosestLandmark / (double)(gAreaWidth*gAreaHeight) );

    // monitoring
    if ( gVerbose && gDisplayMode <= 1 && gMonitorRobot && gRobotIndexFocus == getId() )
    {
        std::cout << "[ Closest landmark : " << std::setw(8) << diffAngleToClosestLandmark << "°, " <<  std::setw(10) << distanceToClosestLandmark << "pix" << " ] --- [ ";
        std::cout << "Sensor values  : " << std::setw(4) << getLandmarkDirectionAngleValue() << " , " << std::setw(4) << getLandmarkDistanceValue() << " ] --- [ ";
        std::cout << "Battery-level  : " << std::setw(6) << getEnergyLevel() << " ] --- [";
        std::cout << "Energy Requested (if asked) : " << std::setw(4) << getEnergyRequestValue() << " ]" << std::endl;
    }
}
