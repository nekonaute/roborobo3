/*
 *  RobotWorldModel.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ROBOTAGENTWORLDMODEL_H
#define ROBOTAGENTWORLDMODEL_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/WorldModel.h"

#include <boost/multi_array.hpp>
#include <iomanip>

class World;

#define SENSOR_REGISTERID 0
#define SENSOR_SOURCENORM 1
#define SENSOR_SOURCEANGLE 2
#define SENSOR_TARGETNORM 3
#define SENSOR_TARGETANGLE 4
#define SENSOR_DISTANCEVALUE 5
#define SENSOR_OBJECTVALUE 6

typedef boost::multi_array<double, 2> sensor_array;

class RobotWorldModel : public WorldModel
{
protected:
    int _groupId; // the robot belongs to a group (default: group #0)
    
    // LED-on-robot: a light displayed at the center of the robot (in display mode).
    // while it may be used for communication, the LED was initialy used to 'tag' the robot for user-purpose.
    // default value is false.
    int _robotLED_red;
    int _robotLED_green;
    int _robotLED_blue;
    
    double _landmarkDirectionAngleValue; // angle to closest landmark -- normalized btw -1 and +1
    double _landmarkDistanceValue ; // distance to the closest landmark -- normalized btw 0  and 1
    double _energyLevel;
    
    double _energyRequestValue; // what the robot wants from an energy point (in [0..1]). Remarks: a robot may not get what it wants (e.g. want 1, get 0 ; want 0, get 100)

    // _cameraSensors :
    //      a collection of sensors. Each sensor contains 7 (double) values:
    //      id sensor(R),sensor_origin_norm,sensor_origin_angle,sensor_target_distance_from_agent_center(!),sensor_target_angle, "current value", object Id.
    sensor_array _cameraSensors;
    
    bool _initSensor;
    
public:

    void initCameraSensors ( int nbSensors );
    
    //sensor(R),sensor_origin_norm,sensor_origin_angle,sensor_target_distance_from_agent_center(!),sensor_target_angle, "current value", object Id.
	double getCameraSensorValue( int sensorId, int valueId )
    {
        return _cameraSensors[sensorId][valueId];
    }
    
    void setCameraSensorValue(  int sensorId, int valueId, double value )
    {
        _cameraSensors[sensorId][valueId] = value;
    }
    
    World *_world;
	
    int _id;
	
    // absolute localization value in the simulated world -- GPS-like (ie. internal data for displaying, not robot-centered)
    double _xReal;
    double _yReal;
    double _agentAbsoluteOrientation;
    double _agentAbsoluteLinearSpeed;
    
    // targeted desired values (impact Absolute Orientation/Linear Speed but bounded by ...MaxDeltaValue(s))
    double _desiredRotationalVelocity;
    double _desiredTranslationalValue;
    
    // delta values that where *actually* executed (ie. may differ if crash occurs)
    double _actualRotationalVelocity;
    double _actualTranslationalValue; // value is equal to _agentAbsoluteLinearSpeed
    
    // max authorized delta values
    double _maxRotationalDeltaValue;		// default: gMaxRotationalSpeed (ie. instantenous)
    double _maxTranslationalDeltaValue;		// default: gMaxTranslationalSpeed (ie. instantenous)
    
    // description and values of sensors
    int _cameraSensorsNb;
    int _groundSensorValue[3]; // r,g,b
    
    int _age;
    int _generation;
	
    double _fitnessValue; // optional
    
	// * Initializes the variables
    RobotWorldModel();
    virtual ~RobotWorldModel();
    
    // * other methods
    
    World* getWorld(); // return pointer to the current World
	
    // * sensor methods
    
    double getDistanceValueFromCameraSensor( int i )
    {
        return getCameraSensorValue(i,SENSOR_DISTANCEVALUE);
    }
    
    double getNormalizedDistanceValueFromCameraSensor( int i )
    {
        return getCameraSensorValue(i,SENSOR_DISTANCEVALUE) / getCameraSensorMaximumDistanceValue(i);
    }
    
    double getObjectIdFromCameraSensor( int i )
    {
        return getCameraSensorValue(i,SENSOR_OBJECTVALUE);
    }
    
    double getCameraSensorMaximumDistanceValue( int i )
    {
        return gSensorRange; // BEWARE: sensor[i][3] is distance btw the end point of the sensor and the **center of the agent**
    }
    
    int getGroundSensor_redValue()
    {
        return _groundSensorValue[0];
    }
    
    int getGroundSensor_greenValue()
    {
        return _groundSensorValue[1];
    }
    
    int getGroundSensor_blueValue()
    {
        return _groundSensorValue[2];
    }
    
    int getGroundSensorValue() // returns 0xRGB
    {
        return _groundSensorValue[0]*256*256 + _groundSensorValue[1]*256 + _groundSensorValue[2];
    }
    
    // * accessing methods
        
    // real absolute X coordinate
    double getXReal()
    {
        return _xReal;
    }
    
    // real absolute Y coordinate
    double getYReal()
    {
        return _yReal;
    }
    
    int getRobotLED_redValue()
    {
        return _robotLED_red;
    }
    
    int getRobotLED_greenValue()
    {
        return _robotLED_green;
    }
    
    int getRobotLED_blueValue()
    {
        return _robotLED_blue;
    }
    
    void setRobotLED_colorValues(int __r, int __g, int __b)
    {
        _robotLED_red = __r;
        _robotLED_green = __g;
        _robotLED_blue = __b;
    }
    
    int getId() { return _id; }
    
    void updateLandmarkSensor(); // update with closest landmark
    void updateLandmarkSensor( int __landmarkId ); // update with a specific landmark
    
    double getLandmarkDirectionAngleValue() { return _landmarkDirectionAngleValue; }
    double getLandmarkDistanceValue() { return _landmarkDistanceValue; }
    void setLandmarkDirectionAngleValue( double value ) { _landmarkDirectionAngleValue = value; }
    void setLandmarkDistanceValue( double value ) { _landmarkDistanceValue = value; }
    
    double getEnergyLevel() { return _energyLevel; }
    void setEnergyLevel( double __value )
    {
        _energyLevel = __value;
        if ( _energyLevel < 0 )
            _energyLevel = 0  ;
        else
            if ( _energyLevel > gEnergyMax )
                _energyLevel = gEnergyMax;
    }
    void addEnergy( double __value )
    {
        _energyLevel += __value;
        if ( _energyLevel > gEnergyMax )
            _energyLevel = gEnergyMax;
    }
    void substractEnergy( double __value )
    {
        _energyLevel -= __value;
        if ( _energyLevel < 0 )
            _energyLevel = 0;
    }

    double getEnergyRequestValue() // when asked, should return how much energy is requested from the robot (between 0 and 1 -- this is guaranteed)
    {
        return _energyRequestValue;
    }
    
    void setEnergyRequestValue( double __value ) // forced between 0 and 1 (truncate if not).
    {
        if ( __value < 0 )
            _energyRequestValue = 0;
        else
            if ( __value > 1.0 )
                _energyRequestValue = 1.0;
            else
                _energyRequestValue = __value;
    }
    
    void setGroupId( int __groupId )
    {
        _groupId = __groupId;
    }
    
    int getGroupId(  )
    {
        return ( _groupId );
    }


};


#endif


