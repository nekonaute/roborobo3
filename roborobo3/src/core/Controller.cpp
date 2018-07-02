/*
 *  Controller.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 19/03/09.
 *
 */

#include "Controllers/Controller.h"
#include "WorldModels/RobotWorldModel.h"
#include "RoboroboMain/roborobo.h"
#include "World/World.h"

Controller::Controller(  )
{
    lastRefreshIteration = -1;
}

Controller::Controller( RobotWorldModel *__wm )
{
    lastRefreshIteration = -1;
	_wm = __wm;
}

Controller::~Controller()
{
	// nothing to do.
}

std::string Controller::inspect( std::string prefix )
{
    return std::string(prefix + "Controller::inspect() not implemented.");
}

/*
  The refreshInputs() method refreshes sensor information, for user by the wrapper access functions.
       In general, calling this method is not called if you access directly sensor information (as in most case)
       This method is useful only if you use wrapper function as defined in Controller.hpp
       Not that this method is automatically called when one of the wrapper function is used, if necessary.
       It can be quite useful to present sensory information in a clear, easy-to-use, format.
  Wrapper access function that use this method are (with "i", the index of the distance sensor):
       int getObject( int i )
       int getObjectType( int i )
       int getRobot( int i )
       int getRobotSamegroupDetector( int i )
       int getWall( int i )
       int getRobotOrientationDetectors( int i )
       double getRedGroundDetectors( )
       double getGreenGroundDetectors( )
       double getBlueGroundDetectors( )
       double getClosestLandmarkDirection()
       double getClosestLandmarkOrientation()
 */
void Controller::refreshInputs(){
    // sensory inputs for each sensor are ordered (and filled in) as follow:
    // - N range sensors
    //      - distance to obstacle (max if nothing) -- in [0,1]
    //      - [0...N_physicalobjecttypes] Is it an object of type i? (0: no, 1: yes) -- type: 0 (round), 1 (energy item), 2 (gate), 3 (switch), ...? (cf. PhysicalObjectFactory)
    //      - is it a robot? (1 or 0)
    //      - is it from the same group? (1 or 0)
    //      - relative orientation wrt. current robot (relative orientation or 0 if not a robot)
    //      - is it a wall? (ie. a non-distinguishible something) (1 or 0)
    // - floor sensor
    //      - red value
    //      - green value
    //      - blue value
    // - relative distance and/or orientation of either the closest landmark (if any, zero if none)
    
    lastRefreshIteration = gWorld->getIterations();
    
    distanceSensors.clear();
    objectDetectors.clear();
    objectTypeDetectors.clear();
    robotDetectors.clear();
    robotSamegroupDetector.clear();
    robotOrientationDetectors.clear();
    wallDetectors.clear();
    
    redGroundDetectors = -1;
    greenGroundDetectors = -1;
    blueGroundDetectors = -1;

    landmark_closest_DirectionDetector = -1;
    landmark_closest_OrientationDetector = -1;
    
    // camera sensors
    
    for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
    {
        distanceSensors.push_back(_wm->getDistanceValueFromCameraSensor(i) / _wm->getCameraSensorMaximumDistanceValue(i));
        
        int objectId = _wm->getObjectIdFromCameraSensor(i);
        
        if ( gSensoryInputs_physicalObjectType )
        {
            // input: physical object? which type?
            if ( PhysicalObject::isInstanceOf(objectId) )
            {
                objectDetectors.push_back( 1 ); // match
                objectTypeDetectors.push_back( gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() );
            }
            else
            {
                // not a physical object. But: should still fill in the inputs (with zeroes)
                int nbOfTypes = PhysicalObjectFactory::getNbOfTypes();
                for ( int i = 0 ; i != nbOfTypes ; i++ )
                {
                    objectDetectors.push_back( 0 );
                }
            }
        }
        
        if ( gSensoryInputs_isOtherAgent )
        {
            // input: another agent? If yes: same group?
            if ( Agent::isInstanceOf(objectId) )
            {
                // this is an agent
                robotDetectors.push_back( 1 );
                
                if ( gSensoryInputs_otherAgentSameGroup )
                {
                    // same group?
                    if ( gWorld->getRobot(objectId-gRobotIndexStartOffset)->getWorldModel()->getGroupId() == _wm->getGroupId() )
                    {
                        robotSamegroupDetector.push_back( 1 ); // match: same group
                    }
                    else
                    {
                        robotSamegroupDetector.push_back( 0 ); // not the same group
                    }
                }
                
                if ( gSensoryInputs_otherAgentOrientation )
                {
                    // relative orientation? (ie. angle difference wrt. current agent)
                    double srcOrientation = _wm->_agentAbsoluteOrientation;
                    double tgtOrientation = gWorld->getRobot(objectId-gRobotIndexStartOffset)->getWorldModel()->_agentAbsoluteOrientation;
                    double delta_orientation = - ( srcOrientation - tgtOrientation );
                    if ( delta_orientation >= 180.0 )
                    {
                        delta_orientation = - ( 360.0 - delta_orientation );
                    }
                    else
                    {
                        if ( delta_orientation <= -180.0 )
                        {
                            delta_orientation = - ( - 360.0 - delta_orientation );
                        }
                    }
                    robotOrientationDetectors.push_back( delta_orientation/180.0 );
                }
            }
            else
            {
                robotDetectors.push_back( 0 ); // not an agent...
                if ( gSensoryInputs_otherAgentSameGroup )
                {
                    robotSamegroupDetector.push_back( 0 ); // ...therefore no match wrt. group.
                }
                if ( gSensoryInputs_otherAgentOrientation )
                {
                    robotOrientationDetectors.push_back( 0 ); // ...and no orientation.
                }
            }
        }
        
        if ( gSensoryInputs_isWall )
        {
            // input: wall or empty?
            if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ) // not empty, but cannot be identified: this is a wall.
            {
                wallDetectors.push_back( 1 );
            }
            else
            {
                wallDetectors.push_back( 0 ); // nothing. (objectId=-1)
            }
        }
        
    }
    
    // floor sensor
    
    if ( gSensoryInputs_groundSensors )
    {
        redGroundDetectors = (double)_wm->getGroundSensor_redValue()/255.0;
        greenGroundDetectors = (double)_wm->getGroundSensor_greenValue()/255.0;
        blueGroundDetectors = (double)_wm->getGroundSensor_blueValue()/255.0;
    }
    
    // closest landmark
    
    if ( gNbOfLandmarks > 0 )  // if ( gSensoryInputs_landmarkTrackerMode == 1 ) // register closest landmark
    {
        _wm->updateLandmarkSensor(); // update with closest landmark
        landmark_closest_DirectionDetector = _wm->getLandmarkDirectionAngleValue();
        landmark_closest_OrientationDetector = _wm->getLandmarkDirectionAngleValue();
    }
    else
    {
        landmark_closest_DirectionDetector = -1;
        landmark_closest_OrientationDetector = -1;
    }

}

bool Controller::checkRefresh()
{
    if ( lastRefreshIteration != gWorld->getIterations() )
        return false;
    else
        return true;
}

// setTranslation(value) takes a value in [-1,+1]
void Controller::setTranslation( double value )
{
    if ( value > 1.0 || value < -1.0 )
    {
        std::cerr << "[WARNING] setTranslation(value) with value out of bounds (" << value << "). Bounding in [-1.0,+1.0]\n";
        if ( value > 1.0 )
        {
            value = 1.0;
        }
        else
        {
            value = -1.0;
        }
    }
    _wm->_desiredTranslationalValue = value * _wm->_maxTranslationalDeltaValue;
}

// setRotation(value) takes a value in [-1,+1]
void Controller::setRotation( double value )
{
    if ( value > 1.0 || value < -1.0 )
    {
        std::cerr << "[WARNING] setRotation(value) with value out of bounds (" << value << "). Bounding in [-1.0,+1.0]\n";
        if ( value > 1.0 )
        {
            value = 1.0;
        }
        else
        {
            value = -1.0;
        }
    }
    _wm->_desiredRotationalVelocity = value * _wm->_maxRotationalDeltaValue;
}

int Controller::getId()
{
    return _wm->getId();
}

double Controller::getActualTranslation()
{
    return _wm->_actualTranslationalValue;
}

double Controller::getActualRotation()
{
    return _wm->_actualRotationalVelocity;
}

// Agent orientation w.r.t. North (ie. upwards) in degrees.
double Controller::getCompass()
{
    return _wm->_agentAbsoluteOrientation;
}
