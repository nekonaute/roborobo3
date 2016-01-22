/*
 *  RobotAgent.cpp
 *  roborobo
 *
 *  Created by Nicolas on 17/12/08.
 *
 */

#include "Config/GlobalConfigurationLoader.h"

#include "Agents/Robot.h"

#include "Utilities/Graphics.h"

#include <iomanip>

Robot::Robot( World *__world )
{
	_wm = gConfigurationLoader->make_RobotWorldModel(); // TODO: externalize object referenced to create the new instance

	_wm->_world = __world;

	_wm->_id = gNumberOfRobots;
	gNumberOfRobots++;
	
	//Process agent specification (ie. IR/US/laser sensors)
	
	// create dynamic array
	// parse image and add (sensor.x/y, orientation) or (sensorStart.x/y, sensorEnd.x/y)	
	
	_wm->_cameraSensorsNb = 0;

	//count sensors
	for ( int x = 0 ; x != gRobotWidth ; x++ ) // image is analysed a first time to count the nuber of sensors (faster than dynamically re-allocating array size for every new sensor)
		for ( int y = 0 ; y != gRobotHeight ; y++ )
		{
			Uint32 pixel = getPixel32( gRobotSpecsImage , x , y);
			if ( pixel != SDL_MapRGBA( gRobotSpecsImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
				_wm->_cameraSensorsNb++;
            /* 
            // DEBUG
            std::cout << "### " << _wm->_cameraSensorsNb << " -- " << pixel << " ("
                << "r="  << ( ( pixel & 0xFF0000 ) >> 16 )
                << ",g=" << ( ( pixel & 0xFF00 ) >> 8 )
                << ",b=" << ( ( pixel & 0xFF ) )
                << ",a=" << ( ( pixel & 0xFF000000 ) >> 24 )
                << ") vs. " << SDL_MapRGBA( gRobotSpecsImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) << "###\n";
             */
		}
    
    _wm->initCameraSensors(_wm->_cameraSensorsNb );
	
    for ( int i = 0 ; i != _wm->_cameraSensorsNb ; i++ )
		_wm->setCameraSensorValue(i,SENSOR_REGISTERID,-1);
	//int sensorIt = 0;
	
	//register sensors
	for ( int x = 0 ; x != gRobotWidth ; x++ )
		for ( int y = 0 ; y != gRobotHeight ; y++ )
		{
			Uint32 pixel = getPixel32( gRobotSpecsImage , x , y);
			if ( pixel != SDL_MapRGBA( gRobotSpecsImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
			{
				// sensor found, register sensor.
				
				Uint8 r, g, b;
				SDL_GetRGB(pixel,gRobotSpecsImage->format,&r,&g,&b); 

				if ( _wm->getCameraSensorValue(r,SENSOR_REGISTERID) != -1 )
				{
					std::cout << "[CRITICAL] robot sensor id already in use -- check agent specification image." << std::endl;
					exit(-1);
				}

				if ( r >= _wm->_cameraSensorsNb )
				{
					std::cout << "[CRITICAL] robot sensor id is not permitted (must be defined btw 0 and " << (_wm->_cameraSensorsNb-1) << ", got: " << r<< ") -- check agent specification image." << std::endl;
					exit(-1);
				}

				
				_wm->setCameraSensorValue(r,SENSOR_REGISTERID,r); // no. sensor
				
				// sensor origin point location wrt. agent center
				_wm->setCameraSensorValue(r,SENSOR_SOURCENORM, sqrt ( (x-gRobotWidth/2) * (x-gRobotWidth/2) + (y-gRobotHeight/2) * (y-gRobotHeight/2) ) ); // length
				double angleCosinus = ( (x-(gRobotWidth/2)) / _wm->getCameraSensorValue(r,SENSOR_SOURCENORM) );
				double angleSinus   =  ( (y-(gRobotHeight/2)) / _wm->getCameraSensorValue(r,SENSOR_SOURCENORM) );
				if (  angleSinus >= 0 )
					_wm->setCameraSensorValue(r,SENSOR_SOURCEANGLE, acos ( angleCosinus ) + M_PI/2 ); // angle (in radian)
				else
					_wm->setCameraSensorValue(r,SENSOR_SOURCEANGLE, -acos ( angleCosinus ) + M_PI/2 + M_PI*2 ); // angle (in radian)

				// sensor target point location wrt. agent center -- sensor target angle is (green+blue) component values
				double angle = g+b-90;   // note: '-90deg' is due to image definition convention (in image, 0° means front of agent, which is upward -- while 0° in simulation means facing right) 				
				double xTarget = ( x - gRobotWidth/2 ) + cos ( angle * M_PI / 180) * gSensorRange;
				double yTarget = ( y - gRobotHeight/2 ) + sin ( angle * M_PI / 180) * gSensorRange;
				_wm->setCameraSensorValue(r,SENSOR_TARGETNORM ,sqrt ( xTarget*xTarget + yTarget*yTarget) ); // length (**from agent center**)
				angleCosinus = xTarget / _wm->getCameraSensorValue(r,SENSOR_TARGETNORM);
				angleSinus   = yTarget / _wm->getCameraSensorValue(r,SENSOR_TARGETNORM);
				if ( angleSinus >= 0 )
					_wm->setCameraSensorValue(r,SENSOR_TARGETANGLE, acos ( angleCosinus ) + M_PI/2 ); // angle (in radian) wrt. agent center
				else
					_wm->setCameraSensorValue(r,SENSOR_TARGETANGLE, -acos ( angleCosinus ) + M_PI/2 + M_PI*2 );
				r++;
			}
		}
    
	_agentObserver = gConfigurationLoader->make_AgentObserver(_wm);		// TODO: externalize
	_controller = gConfigurationLoader->make_Controller(_wm);		// TODO: externalize

	reset();
}


Robot::~Robot()
{
	delete _controller;
	delete _agentObserver;
}

void Robot::reset()
{
    //Initialize general
	_iterations = 0;
	_wm->setAlive(true);

	int x = 0 , y = 0;

	std::stringstream out;
	out << _wm->getId();

    std::string str_groupId = "";
	str_groupId += "robot[";
	str_groupId += out.str();
	str_groupId += "].groupId";
    
    if ( gProperties.hasProperty( str_groupId ) == true )
    {
        int groupId;
		convertFromString<int>(groupId, gProperties.getProperty( str_groupId ), std::dec);
        if ( groupId >= gNumberOfRobotGroups || groupId < 0 )
        {
            std::cerr << "[CRITICAL] robot #" << _wm->getId() << ": groupId (" << groupId << ") must be in [0,gNumberOfRobotGroups=" << gNumberOfRobotGroups << "[. EXITING.";
            exit(-1);
        }
        _wm->setGroupId(groupId);
    }
    else
        _wm->setGroupId(0);
    
	std::string str_Xcoord = "";
	str_Xcoord += "robot[";
	str_Xcoord += out.str();
	str_Xcoord += "].x";

	std::string str_Ycoord = "";
	str_Ycoord = "robot[";
	str_Ycoord += out.str();
	str_Ycoord += "].y";
    
    int tries = 0;
    int maxTries = gLocationFinderMaxNbOfTrials;
    bool randomPick = true;

	if ( gProperties.hasProperty( str_Xcoord ) == true && gProperties.hasProperty( str_Ycoord ) == true )
	{
		convertFromString<int>(x, gProperties.getProperty( str_Xcoord ), std::dec);
		convertFromString<int>(y, gProperties.getProperty( str_Ycoord ), std::dec);
        randomPick = false;
	}
	else
	{	
		bool success;

		do {
			success = true;
		
			// pick random coordinate
			
			x = (int)(ranf() * (double)gAreaWidth);
			y = (int)(ranf() * (double)gAreaHeight);
			
			// check for agents superposition - ie. if picked position is valid vs. already located agents.
			for ( int i = 0 ; i != _wm->getId() ; i++ )
			{
                if ( ( std::abs((double)x - gWorld->getRobot(i)->_wm->_xReal) <= gRobotWidth+1 ) && ( std::abs((double)y - gWorld->getRobot(i)->_wm->_yReal) <= gRobotHeight+1 ) ) // uses square boxes as location approximation
				{
					success = false;
					break; // terminate for statement.
				}
			}

			if ( success == false )
			{
				continue; // no need to perform next check...
			}
			
			// check if position is valid in environment
			
			for ( int i = x - gRobotWidth/2 ; i <= x + gRobotWidth/2 ; i++ )
			{
				for ( int j = y - gRobotHeight/2 ; j <= y + gRobotHeight/2 ; j++ ) // && valid == true
				{
					// get pixel values
					Uint8 r, g, b;
					Uint32 pixel = getPixel32( gEnvironmentImage, i , j);
					SDL_GetRGB(pixel,gEnvironmentImage->format,&r,&g,&b); 
					
					int color = ((r<<16)+(g<<8)+b);
					
					// check if empty
					if ( color != ((255<<16)+(255<<8)+255) ) // r=robot, g=obstacle/object, b=unused
					{
						//std::cout << "faux: " << i << "," << j << " - " << ((r<<16)+(g<<8)+b) << std::endl; //<< " - " << (int)r << " " << (int)g << " " << (int)b << std::endl;
						success = false;
					}
					//else std::cout << "ok!: "  << j << "," << j << " - " << ((r<<16)+(g<<8)+b) << std::endl; //<< " - " << (int)r << " " << (int)g << " " << (int)b << std::endl;
				}
			}

			tries++;
				
		} while ( success == false && tries < maxTries );
			
		if ( tries == gLocationFinderMaxNbOfTrials )
		{
            std::cerr << "[CRITICAL] Random initialization of initial position for agent #" << _wm->getId() << " after trying " << maxTries << " random picks (all failed). There may be too few (none?) possible locations (you may try to manually set initial positions). EXITING.\n";
			exit(-1);
		}
    }

	setCoordReal(x,y);

    //Initialize coordinate and displacement
	_xDelta = 0;
	_yDelta = 0;

	//Initialize internal variables
	_xDeltaReal=0;
	_yDeltaReal=0;
		
    // * set orientation
    
    //bool randomOrientation = false;
    
	std::string str_orient = "";
	str_orient = "robot[";
	str_orient += out.str();
	str_orient += "].orientation";
	if ( gProperties.hasProperty( str_orient ) )
    {
		convertFromString<double>(_wm->_agentAbsoluteOrientation, gProperties.getProperty( str_orient ), std::dec);
    }
	else
	{
		_wm->_agentAbsoluteOrientation = ranf() * 360. - 180.;
        //randomOrientation = true;
	}
    
    // display results
    
    if ( gVerbose )
    {
        
        std::cout << "[INFO] Robot #" << _wm->getId() << " positioned at ( "<< std::setw(5) << x << " , " << std::setw(5) << y << " ) with orientation " << std::setw(8) << _wm->_agentAbsoluteOrientation << "° -- ";
        
        if ( randomPick == false )
            std::cout << "[user-defined position]\n";
        else
        {
            std::cout << "[random pick after " << tries;
            if ( tries <= 1 )
                std::cout << " try]\n";
            else
                std::cout << " tries]\n";
            
        }
    }
    
    // set other parameters
	
	_wm->_agentAbsoluteLinearSpeed = 0;

	_wm->_desiredRotationalVelocity = 0;
	_wm->_desiredTranslationalValue = 0;
	
	_wm->_maxRotationalDeltaValue = gMaxRotationalSpeed; // ie. change will be instantenous
	_wm->_maxTranslationalDeltaValue = gMaxTranslationalDeltaValue; // idem.
	
	_wm->_actualTranslationalValue = 0;
	_wm->_actualRotationalVelocity = 0;

	for ( int i = 0 ; i != _wm->_cameraSensorsNb ; i++ ) // initialize sensor values to max range, no contact
	{
		_wm->setCameraSensorValue(i,SENSOR_DISTANCEVALUE, gSensorRange ); // range: max
		_wm->setCameraSensorValue(i,SENSOR_OBJECTVALUE, 0 ); // type:  none
	}
    
    for ( int i = 0 ; i < 3 ; i++ )
        _wm->_groundSensorValue[i] = 0; // floor sensor value (taken from gGroundSensorImage)
	
	// Initialize agent observer and Behavior Control Architecture


  
	_agentObserver->reset();
	_controller->reset();

}

void Robot::callObserver()
{
	_agentObserver->step();
}

void Robot::stepBehavior()
{
	// update status
	_iterations++;
	
    // compute commands
    _controller->step(); // whether alive or not.

	displayInfo();
}


void Robot::stepBehavior(const Uint8* __keyboardStates)
{
	_iterations++;

	// compute commands
	behaviorUpdate_remoteController(__keyboardStates);

	displayInfo();
}

void Robot::displayInfo()
{
	if ( gVerbose && gDisplayMode <= 1)
    {
		if ( gMonitorRobot && gRobotIndexFocus == _wm->getId() )
		{
			std::cout << "Robot #" << _wm->getId() << ": ( " ;
			for ( int i = 0 ; i != _wm->_cameraSensorsNb ; i++ )
			{
				std::cout << _wm->getDistanceValueFromCameraSensor(i) << "/" <<  _wm->getCameraSensorMaximumDistanceValue(i) << " ";
			}
			std::cout << ") , ( groundSensor: " << _wm->getGroundSensorValue() << " [" << _wm->getGroundSensor_redValue() << "," << _wm->getGroundSensor_greenValue() << "," << _wm->getGroundSensor_blueValue() << "] ) => ( " <<  _wm->_desiredTranslationalValue << " ; " << _wm->_desiredRotationalVelocity << " ) (LED: " << _wm->getRobotLED_redValue() << "," << _wm->getRobotLED_greenValue() << "," << _wm->getRobotLED_blueValue() << ") \n";
        
            std::cout << "[_";
            for( int i = 0 ; i < _wm->_cameraSensorsNb; i++)
            {
                double value = _wm->getObjectIdFromCameraSensor(i);
                
                if ( value == -1 )
                    std::cout << " nothing    _";
                else
                    if ( value >= gRobotIndexStartOffset)
                        std::cout << " rob" << std::setw(7) << value-gRobotIndexStartOffset << " _";
                    else
                        if ( value >= gPhysicalObjectIndexStartOffset )
                            std::cout << " obj" << std::setw(7) << value-gPhysicalObjectIndexStartOffset << " _";
                        else
                            std::cout << " wall "<< std::setw(6) << value << " _";
            }
            
            std::cout << "] " << std::endl;
		}
    }
}

void Robot::behaviorUpdate_remoteController(const Uint8* __keyboardStates) // user controlled robot
{
	if ( gRobotIndexFocus != _wm->getId() )
		return;

	// key pressed
	gKeyUp = __keyboardStates[ SDL_SCANCODE_UP ];
	gKeyDown = __keyboardStates[ SDL_SCANCODE_DOWN ];
	gKeyLeft = __keyboardStates[ SDL_SCANCODE_LEFT ];
	gKeyRight = __keyboardStates[ SDL_SCANCODE_RIGHT ];

	// update speed

	if ( gKeyUp )
		_wm->_desiredTranslationalValue += 1; // pixels
	if ( gKeyDown )
		_wm->_desiredTranslationalValue += -1;

	if ( _wm->_desiredTranslationalValue > gMaxTranslationalSpeed )
		 _wm->_desiredTranslationalValue = gMaxTranslationalSpeed;
	else
		if ( _wm->_desiredTranslationalValue < -gMaxTranslationalSpeed )
			_wm->_desiredTranslationalValue = -gMaxTranslationalSpeed;
		 
	if ( gKeyLeft )
		_wm->_desiredRotationalVelocity = -5; // degrees
	if ( gKeyRight )
		_wm->_desiredRotationalVelocity = 5;
		
	if ( !gKeyUp && !gKeyDown )
		_wm->_desiredTranslationalValue = 0;
		
	if ( !gKeyRight && !gKeyLeft )
	{
		if ( _wm->_desiredRotationalVelocity > 0 ) 
			_wm->_desiredRotationalVelocity--;
		else
			if ( _wm->_desiredRotationalVelocity < 0 ) 
				_wm->_desiredRotationalVelocity++;
	}

	if ( __keyboardStates[SDL_SCANCODE_O] )
	{
		_wm->_desiredTranslationalValue = 0;
		_wm->_desiredRotationalVelocity = 0;
	}
}

/*
	Compute actual Translational value and Rotational velocity that can be applied given the agent/robot dynamics.
	By default, this means: apply min/max values bounds, apply threshold on maximum delta values wrt. current values.
	By extension, this means: also apply some specific environment impact on the agent/robot dynamics (e.g. slow down due to sticky terrain, etc.). Details:
	1. convert desired translation/rotation to possible translation/rotation values
	2. apply environment contraints (call external method)
	3. translate into internal delta position update (ie. 2d x/y delta values)
*/
void Robot::applyDynamics() 
{
	
	_lastAgentAbsoluteOrientation = _wm->_agentAbsoluteOrientation; // in case of collision, original orientation should be restored.
	
	// * translation
	// assume controller gives desired speed (the following models a feedback system (fr.: systeme asservi))
	
	_wm->_actualTranslationalValue = _wm->_agentAbsoluteLinearSpeed;
	
	if ( _wm->_desiredTranslationalValue != _wm->_agentAbsoluteLinearSpeed )
	{
		double diff = fabs( _wm->_desiredTranslationalValue - _wm->_agentAbsoluteLinearSpeed ); // Note: using abs(...) would truncate to integer. Use fabs instead.

		if ( _wm->_desiredTranslationalValue > _wm->_agentAbsoluteLinearSpeed )
		{
			if ( diff > _wm->_maxTranslationalDeltaValue )
            {
				_wm->_actualTranslationalValue += _wm->_maxTranslationalDeltaValue;
            }
			else
                if ( diff <= _wm->_maxTranslationalDeltaValue )
                {
                    _wm->_actualTranslationalValue += diff;
                }
                else
                {
                    std::cout << "[ERROR] Robot::applyDynamics() : _wm->_desiredTranslationalValue is Nan (probably a bug in your project). STOPPING.\n";
                    exit(-1);
                }
		}
		else // strictly below
		{
			if ( diff > _wm->_maxTranslationalDeltaValue )
            {
				_wm->_actualTranslationalValue -= _wm->_maxTranslationalDeltaValue;
            }
			else
                if ( diff <= _wm->_maxTranslationalDeltaValue )
                {
                    _wm->_actualTranslationalValue -= diff;
                }
                else
                {
                    std::cout << "[ERROR] Robot::applyDynamics() : _wm->_desiredTranslationalValue is Nan (probably a bug in your project). STOPPING.\n";
                    exit(-1);
                }

		}

		// upper and lower bound for translation speed
		if ( _wm->_actualTranslationalValue > gMaxTranslationalSpeed )
        {
			_wm->_actualTranslationalValue = gMaxTranslationalSpeed;
        }
		else
        {
			if ( _wm->_actualTranslationalValue < -gMaxTranslationalSpeed )
            {
				_wm->_actualTranslationalValue = -gMaxTranslationalSpeed;
            }
        }
	}
	
	// * rotation
	// assume controller gives angular velocity.

	if ( _wm->_desiredRotationalVelocity > 0 )
	{
		if ( _wm->_desiredRotationalVelocity <= _wm->_maxRotationalDeltaValue )
			_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
		else
			_wm->_actualRotationalVelocity = _wm->_maxRotationalDeltaValue;
	}
	else
		if ( _wm->_desiredRotationalVelocity < 0 )
		{
			if ( _wm->_desiredRotationalVelocity >= -_wm->_maxRotationalDeltaValue )
				_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
			else
				_wm->_actualRotationalVelocity = -_wm->_maxRotationalDeltaValue;
		}
        else
            if ( _wm->_desiredRotationalVelocity == 0 )
                _wm->_actualRotationalVelocity = 0;
            else
            {
                std::cout << "[ERROR] Robot::applyDynamics() : _wm->_desiredRotationalVelocity is Nan (probably a bug in your project). STOPPING.\n";
                exit(-1);
            }
    
	// * compute true displacement (actual (thought to be true) and true (really effected) rotation and translation,

	// update both actual trans/rot values and internal data (see method definition for detailed comments)
	// after this call, wm->_agentAbsoluteLinearSpeed AND _wm->_agentAbsoluteOrientation are updated (internal data)
	applyRobotPhysics(); //applyInternalDynamics is a static function

	// note: the next locomotion-related method called will be the "move" method. It handles collision.
	// In case of collision, actual translation/rotation variables will be updated accordingly
	// Keep in mind that roborobo philosophy assumes pixel-based level of details for collision.
}




 /**
    * update the agent position in the environment. Apply simple physics (ie. obstacle collision detection and consequences).
	* if collision, translation and rotation speed are set to zero.
    * note: __recursiveIt is currently unused (maybe used for debug purpose, eg. checking for infinite loop.)
    */
void Robot::move( int __recursiveIt ) // the interface btw agent and world -- in more complex envt, this should be handled by the "world".
{
	// apply world dynamics onto this agent

	// * compute real valued delta (convert to x/y delta coordinates)
    
	applyDynamics();

	// * save position
    
    double xReal_old = _wm->_xReal; // backup old position in case of collision
	double yReal_old = _wm->_yReal;
	
	
	// * update x/y position
		
	_xDeltaReal = _wm->_agentAbsoluteLinearSpeed * cos(_wm->_agentAbsoluteOrientation * M_PI / 180);
	_yDeltaReal = _wm->_agentAbsoluteLinearSpeed * sin(_wm->_agentAbsoluteOrientation * M_PI / 180);

	_wm->_xReal += _xDeltaReal;
	_wm->_yReal += _yDeltaReal;	// TODO: round is for positive values... (ok in this case however as 0,0 is up-left)

	setCoord((int)_wm->_xReal+0.5,(int)_wm->_yReal+0.5);
	
	// * collision with (image) border of the environment - position at border, then bounce
    
	if ( isCollision() )
	{
		_wm->_desiredTranslationalValue = 0; // cancel any translation order as agent translation speed is set to zero after collision. (note that rotation is still ok)
		
		if (_wm->_agentAbsoluteLinearSpeed >= 1.0 )
		{
			_wm->_agentAbsoluteLinearSpeed--;
		}
		else
		{
			if ( _wm->_agentAbsoluteLinearSpeed <= -1.0 )
				_wm->_agentAbsoluteLinearSpeed++;
		}
		
		if ( ( _wm->_agentAbsoluteLinearSpeed > -1.0 ) && ( _wm->_agentAbsoluteLinearSpeed < 1.0 ) ) 
		{
			_wm->_agentAbsoluteLinearSpeed = 0; // roborobo assumes pixel-based resolution for solving collision. Ie. ignore sub-pixel translation values. (see _README.TXT for details)
		}
		
		_wm->_xReal=xReal_old;
		_wm->_yReal=yReal_old;
		
		if (_wm->_agentAbsoluteLinearSpeed != 0 ) // if zero: move is canceled
		{
			__recursiveIt++;
			move(__recursiveIt);
		}
		else
		{
			// special case: agent cannot not move at all - restore original coordinate (remember, _x/yReal are global and modified during recursive call).
			_wm->_xReal=xReal_old;
			_wm->_yReal=yReal_old;			
			setCoord((int)_wm->_xReal+0.5,(int)_wm->_yReal+0.5);
		}

		// update world model variables and internal data
		
		_wm->_agentAbsoluteLinearSpeed = 0;
		
		if ( gLocomotionMode == 1 ) // consider obstacle friction or not for rotation?
		{
			_wm->_agentAbsoluteOrientation = _lastAgentAbsoluteOrientation;
			_wm->_actualRotationalVelocity = 0;
		}
		else
			_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
			
		//_wm->_desiredTranslationalValue = _wm->_desiredRotationalVelocity = 0;
		_wm->_actualTranslationalValue = 0;

	}
	else
	{
		// actual rotational and translational values matches desired values
		_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
		_wm->_actualTranslationalValue = _wm->_agentAbsoluteLinearSpeed; // (!) _wm->_desiredTranslationalValue is different as the "desired" translation speed may not be reached due to physical actuator limitations
	}
	
	// * update sensors
    
	for ( int i = 0 ; i != _wm->_cameraSensorsNb ; i++ )
	{
		// Warning: the following is repeated in the show method because coordinates are not stored, but are needed to display the sensor rays.
		double x1 = (_wm->_xReal + _wm->getCameraSensorValue(i,SENSOR_SOURCENORM) * cos( _wm->getCameraSensorValue(i,SENSOR_SOURCEANGLE) + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
		double y1 = (_wm->_yReal + _wm->getCameraSensorValue(i,SENSOR_SOURCENORM) * sin( _wm->getCameraSensorValue(i,SENSOR_SOURCEANGLE) + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
		double x2 = (_wm->_xReal + _wm->getCameraSensorValue(i,SENSOR_TARGETNORM) * cos( _wm->getCameraSensorValue(i,SENSOR_TARGETANGLE) + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
		double y2 = (_wm->_yReal + _wm->getCameraSensorValue(i,SENSOR_TARGETNORM) * sin( _wm->getCameraSensorValue(i,SENSOR_TARGETANGLE) + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;

		// cast sensor ray.
		_wm->setCameraSensorValue(i,SENSOR_DISTANCEVALUE, castSensorRay(gEnvironmentImage, x1, y1, &x2, &y2, _wm->getCameraSensorMaximumDistanceValue(i)) ); // x2 and y2 are overriden with collision coordinate if ray hits object. -- not used here.
        
		Uint8 r, g, b;
		Uint32 pixel = getPixel32( gEnvironmentImage, x2 , y2);
		SDL_GetRGB(pixel,gEnvironmentImage->format,&r,&g,&b);
        if ( r == 0xFF && g == 0xFF && b == 0xFF )
            _wm->setCameraSensorValue(i,SENSOR_OBJECTVALUE, -1); // nothing
        else
            _wm->setCameraSensorValue(i,SENSOR_OBJECTVALUE, (r<<16)+(g<<8)+b); // type of objects. [0-gRobotIndexStartOffset[ is object, [gRobotIndexStartOffset-...[ is robots
            //_wm->setCameraSensorValue(i,SENSOR_OBJECTVALUE, (r<<16)+(g<<8)+b); // type of objects. [0-gRobotIndexStartOffset[ is object, [gRobotIndexStartOffset-...[ is robots
	}
    
	Uint8 r, g, b;
	Uint32 pixel = getPixel32( gGroundSensorImage, _wm->_xReal+0.5, _wm->_yReal+0.5);
	SDL_GetRGB(pixel,gGroundSensorImage->format,&r,&g,&b); 
	_wm->_groundSensorValue[0] = r;
	_wm->_groundSensorValue[1] = g;
	_wm->_groundSensorValue[2] = b;
}

/* Check collision between the robot and the environment (gEnvironmentImage - contains robots, physical objects, walls)
*/
bool Robot::isCollision()
{
	// check collision with borders and environment objects.
    if ( 
		( _x < 0 ) || ( _x + gRobotWidth >= gAreaWidth ) ||
		( _y < 0 ) || ( _y + gRobotHeight >= gAreaHeight )
	   )
	{
		// * collision with border 
		return true;
	}
	else
	{
		// * environment objects 
		for ( int i = 0 ; i != gRobotWidth ; i++ )
			for ( int j = 0 ; j != gRobotHeight ; j++ )
			{
				if ( getPixel32( gRobotMaskImage , i , j) != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ) // opt: bounding box instead of pixel-to-pixel test.
				{
                    // not useful: testing out-of-the-world status
                    // if ( ( _x + i < 0 ) || ( _x + i  >= gAreaWidth ) || ( _y + j < 0 ) || ( _y + i  >= gAreaHeight ) ) { return true;	}				
					Uint32 pixel = getPixel32( gEnvironmentImage , _x+i , _y+j);
					if (  pixel != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
					{
						return true;
					}
				}
			}
	}	
	
	return false;
}


 /**
    * Display agent on screen. Add information caption if needed.
	* 
	* (render mode only) 
    */
void Robot::show() // display on screen
{    
    //Show the dot
    
	if ( gNiceRendering )
		apply_surface( _x - gCamera.x, _y - gCamera.y, gRobotDisplayImage, gScreen ); // OPTIONAL (agent is already visible/registered through the environment image -- but: may be useful for image capture

	if ( gRobotLEDdisplay == true )
	{
			int dx = 1;
			int dy = 1;
			int xcenter = (int)_wm->_xReal + 0.5;
			int ycenter = (int)_wm->_yReal + 0.5;
			int r = getWorldModel()->getRobotLED_redValue();
			int g = getWorldModel()->getRobotLED_greenValue(); 
			int b = getWorldModel()->getRobotLED_blueValue();
			
			for ( int xTmp = xcenter - dx; xTmp != xcenter + dx + 1 ; xTmp++ )
				for ( int yTmp = ycenter - dy - 1 ; yTmp != ycenter + dy ; yTmp++ )
				{
					putPixel32_secured( gScreen, xTmp - gCamera.x, yTmp + dy - gCamera.y , SDL_MapRGBA( gScreen->format, r, g, b, SDL_ALPHA_OPAQUE ) );
				}		
	}


	if ( _wm->getId() == gRobotIndexFocus && ( gUserCommandMode || gRobotDisplayFocus ) ) // && _iterations%10 < 5) 
	{
			int dx = 10;
			int dy = 10;
			int xcenter = (int)_wm->_xReal + 0.5;
			int ycenter = (int)_wm->_yReal + 0.5;
			int r = 255.0 * ranf();
			int g = 255.0 * ranf();
			int b = 255.0 * ranf();
						
			for ( int xTmp = xcenter - dx ; xTmp != xcenter + dx + 1 ; xTmp++ )
			{
				putPixel32( gScreen, xTmp - gCamera.x, ycenter - dy - gCamera.y , SDL_MapRGBA( gScreen->format, r, g, b, SDL_ALPHA_OPAQUE ) );
				putPixel32( gScreen, xTmp - gCamera.x, ycenter + dy - gCamera.y , SDL_MapRGBA( gScreen->format, r, g, b, SDL_ALPHA_OPAQUE ) );
			}

			for ( int yTmp = ycenter - dy ; yTmp != ycenter + dy + 1 ; yTmp++ )
			{
				putPixel32( gScreen, xcenter - dx - gCamera.x, yTmp - gCamera.y , SDL_MapRGBA( gScreen->format, r, g, b, SDL_ALPHA_OPAQUE ) );
				putPixel32( gScreen, xcenter + dx - gCamera.x, yTmp - gCamera.y , SDL_MapRGBA( gScreen->format, r, g, b, SDL_ALPHA_OPAQUE ) );
			}
	}

    // * show sensors and orientation
    
	if ( gDisplaySensors == 1 || gDisplaySensors == 2 || gDisplaySensors == 3 )
	{
		// * show sensors

		for ( int i = 0 ; i != _wm->_cameraSensorsNb ; i++ )
		{
			// Warning: the following is a repetition of code already in the move method (sensor ray casting) in order to display it (coordinates are not stored)
			double x1 = (_wm->_xReal + _wm->getCameraSensorValue(i,SENSOR_SOURCENORM) * cos( _wm->getCameraSensorValue(i,SENSOR_SOURCEANGLE) + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
			double y1 = (_wm->_yReal + _wm->getCameraSensorValue(i,SENSOR_SOURCENORM) * sin( _wm->getCameraSensorValue(i,SENSOR_SOURCEANGLE) + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
			double x2 = (_wm->_xReal + _wm->getCameraSensorValue(i,SENSOR_TARGETNORM) * cos( _wm->getCameraSensorValue(i,SENSOR_TARGETANGLE) + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
			double y2 = (_wm->_yReal + _wm->getCameraSensorValue(i,SENSOR_TARGETNORM) * sin( _wm->getCameraSensorValue(i,SENSOR_TARGETANGLE) + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;

			// sensor ray casting is also performed in the move method -- this time we dont store data (already done). -- this one is only used to *display* the ray. _cameraSensors[i][5] contains the length, but not the (x2,y2) location where the ray stops.
            castSensorRay(gEnvironmentImage, x1, y1, &x2, &y2, _wm->getCameraSensorMaximumDistanceValue(i)); // x2 and y2 are overriden with collision coordinate if ray hits object.

			// display on screen
			if ( _wm->getCameraSensorValue(i,SENSOR_DISTANCEVALUE) < _wm->getCameraSensorMaximumDistanceValue(i)-1 ) //gSensorRange-1 )
            {
                if ( gDisplaySensors == 2 )
                    traceRayRGB(gScreen, int(x1+0.5)-gCamera.x, int(y1+0.5)-gCamera.y, int(x2+0.5)-gCamera.x, int(y2+0.5)-gCamera.y, 255 , 0 , 0 );
                else
                    traceRayRGB(gScreen, int(x1+0.5)-gCamera.x, int(y1+0.5)-gCamera.y, int(x2+0.5)-gCamera.x, int(y2+0.5)-gCamera.y, 192 , 192 , 255 );
            }
			else
            {
                if ( gDisplaySensors == 2 || gDisplaySensors == 3 )
                    traceRayRGB(gScreen, int(x1+0.5)-gCamera.x, int(y1+0.5)-gCamera.y, int(x2+0.5)-gCamera.x, int(y2+0.5)-gCamera.y, 192 , 192 , 255 );
            }
		}
        
    }
    
    if ( _wm->getId() == gRobotIndexFocus && gUserCommandMode )
    {
        // * user is taking control of targeted agent - show agent and orientation with multiple changing colors
        
        int xOrientationMarkerSource =  (int)(_wm->_xReal);
        int yOrientationMarkerSource =  (int)(_wm->_yReal);
        
        int xOrientationMarkerTarget =  (int)(_wm->_xReal) + gSensorRange*0.75 * cos(( _wm->_agentAbsoluteOrientation + 180 ) * M_PI / 180);
        int yOrientationMarkerTarget =  (int)(_wm->_yReal) + gSensorRange*0.75 * sin(( _wm->_agentAbsoluteOrientation + 180 ) * M_PI / 180);
        
        int r,g,b;
        g = b = (32*_iterations%256) > 128 ? 0 : 255 ;
        r = 0;
        
        // show orientation (multicolor) - this is done by adding a *virtual* tail *behind* the robot
        for ( int xTmp = -1 ; xTmp < 2 ; xTmp+=2 )
            for ( int yTmp = -1 ; yTmp < 2 ; yTmp+=2 )
                traceRayRGB(gScreen, (int)(_wm->_xReal+(double)xTmp)  - gCamera.x, (int)(_wm->_yReal+(double)yTmp) - gCamera.y, xOrientationMarkerTarget - gCamera.x, yOrientationMarkerTarget - gCamera.y, r , g , b );
        
        // show position (multicolor)
        for ( int xTmp = -2 ; xTmp != 3 ; xTmp++ )
            for ( int yTmp = -2 ; yTmp != 3 ; yTmp++ )
                putPixel32( gScreen, xOrientationMarkerSource - gCamera.x + xTmp, yOrientationMarkerSource - gCamera.y + yTmp , SDL_MapRGB( gScreen->format, r, b , g ) );
        
    }
    else
    {
        // * show orientation - this is done by adding a *virtual* tail *behind* the robot
        if ( gDisplayTail && gNiceRendering )
        {
            int xOrientationMarkerTarget =  (int)(_wm->_xReal) + gTailLength * cos(( _wm->_agentAbsoluteOrientation + 180 ) * M_PI / 180);
            int yOrientationMarkerTarget =  (int)(_wm->_yReal) + gTailLength * sin(( _wm->_agentAbsoluteOrientation + 180 ) * M_PI / 180);

            for ( int xTmp = -1 ; xTmp < 2 ; xTmp+=2 )
                for ( int yTmp = -1 ; yTmp < 2 ; yTmp+=2 )
                    traceRayRGB(gScreen, (int)(_wm->_xReal+(double)xTmp)  - gCamera.x, (int)(_wm->_yReal+(double)yTmp) - gCamera.y, xOrientationMarkerTarget - gCamera.x, yOrientationMarkerTarget - gCamera.y, 90, 113, 148 ); // 255 , 128 , 0
        }
        
    }
    
}

void Robot::registerRobot()
{
    // [!n] CONVENTION: _agentId+gRobotIndexStartOffset to avoid confusion with empty space (0: empty ; 1: static object; 2-99: reserved for other static objects)
    register_robotMask( _x, _y, gEnvironmentImage, _wm->getId()+gRobotIndexStartOffset );
}

void Robot::unregisterRobot()
{
    clean_robotMask( _x, _y, gEnvironmentImage );
}

void Robot::setCoordReal (int __x, int __y) // agent is centered on point
{
	//setCoord(gRobotXStart+__x,gRobotYStart+__y);
	//setCoord(__x,__y);
	_wm->_xReal = __x;
	_wm->_yReal = __y;
}

/**
 trace (sensor) ray from (x1,y1) to (x2,y2) whatever the obstacles.
 used for rendering to screen (not critical wrt simulation).
 */
void Robot::traceRayRGB(SDL_Surface * image, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b)
{
    drawLine( image, x1, y1, x2, y2, r, g, b );
    
    /*
    // Method used before 2015-10-26 -- (slightly) more accurate, but (marginally) slower (approx.: 0.9*new_method)
    // Kept here for the record -- can be deleted in 2016.
    
    Uint32 color =SDL_MapRGBA( image->format, r, g, b, SDL_ALPHA_OPAQUE );
     
	if ( abs(x1-x2) > abs (y1-y2) )
	{
		int it;
		
		double dy = double(y1-y2) / double(x1-x2);
        
		if ( (x1-x2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dy=-dy;
		}
		
		double yReal = y1;
		
		for ( int x = x1 ; x != x2 ; x = x + it )
		{
			putPixel32_secured( image, x, (int)(yReal+0.5) , color );
			yReal += dy;
		}
	}
	else
	{
		int it;
		
		double dx = double(x1-x2) / double(y1-y2);
        
		if ( (y1-y2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dx=-dx;
		}
		
		double xReal = x1;
		
		for ( int y = y1 ; y != y2 ; y = y + it )
		{
			putPixel32_secured( image, (int)(xReal+0.5), y , color );
			xReal += dx;
		}
	}
    */
}

/**
 cast (sensor) ray from (x1,y1) to (x2,y2). Stops whenever ray encounters something. (x2,y2) are update with point of contact
 __maxValue is the maximum distance possible -- ie. if no collision during ray casting (makes it possible to return an exact value without the cost of distance (with sqrt) computation)
 */
int Robot::castSensorRay(SDL_Surface * image, double x1, double y1, double *x2pt, double *y2pt, int __maxValue )
{
    /**/
    int x2 = (int)(*x2pt + 0.5);
    int y2 = (int)(*y2pt + 0.5);
    
    bool collision = castLine(image, (int)(x1+0.5), (int)(y1+0.5), &x2, &y2, __maxValue);
    
    *x2pt = (double)x2;
    *y2pt = (double)y2;
    
    if ( collision )
        return sqrt ( ( x1 - *x2pt ) * ( x1 - *x2pt ) + ( y1 - *y2pt ) * ( y1 - *y2pt ) );
    else
        if ( __maxValue != -1 )
            return __maxValue;
        else
            return gSensorRange;
    
    /*
    // Method used before 2015-10-26 -- (slightly) more accurate, but (marginally) slower (approx.: 0.9*new_method)
    // Kept here for the record -- can be deleted in 2016.
     
	double x2 = *x2pt;
	double y2 = *y2pt;
	
	bool isCollision = false; // check collision btw sensor ray and object.
	
	if ( std::abs(x1-x2) > std::abs (y1-y2) )
	{
		int it;
		
		double dy = (y1-y2) / (x1-x2);
        
		if ( (x1-x2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dy=-dy;
		}
		
		double yReal = y1;
		
		for ( int x = (int)(x1+0.5) ; x != (int)(x2+0.5) ; x = x + it )
		{
			if ( getPixel32 ( image, x, (int)(yReal+0.5) ) != SDL_MapRGBA( image->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
			{
				*x2pt = (double)x;
				*y2pt = yReal;
				isCollision = true;
				break;
			}
			yReal += dy;
		}
	}
	else
	{
		int it;
		
		double dx = (x1-x2) / (y1-y2);
        
		if ( (y1-y2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dx=-dx;
		}
		
		double xReal = x1;
		
		for ( int y = (int)(y1+0.5) ; y != (int)(y2+0.5) ; y = y + it )
		{
			if ( getPixel32 ( image, (int)(xReal+0.5), y ) != SDL_MapRGBA( image->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) )
			{
				*x2pt = xReal;
				*y2pt = (double)y;
				isCollision = true;
				break;
			}
			xReal += dx;
		}
	}
    
	if ( isCollision == false && __maxValue != -1 )
		return __maxValue;
	else
		return sqrt ( ( x1 - *x2pt ) * ( x1 - *x2pt ) + ( y1 - *y2pt ) * ( y1 - *y2pt ) );
    // should be equal to gSensorRange; // no hit
     
    */
}


/*
 This method prepares agents for applying dynamics. It is called by the World->updateWorld method only once per world iteration.
 */
void Robot::initRobotPhysics ()
{
	// nothing to do.
}

/*
 This method apply internal world dynamics onto actual effector values (ie. values that comply
 with controller hardware limitations). In this method, two important steps are performed:
 1. update actual translation/rotation values wrt. to environmental constraints (optional)
 e.g.: add noise to motor execution that can be perceived back,
 interpretation: actual values are also used as proprioceptive sensors
 2. compute internal data for moving the robot (should not be accessed by the robot)
 e.g.: slippery terrain, proprioceptive sensor defect, etc.
 interpretation: all the world dynamics that cannot be grasped through proprioceptive sensors
 */
void Robot::applyRobotPhysics( )
{
	// * update internal data
    
    _wm->_agentAbsoluteLinearSpeed = _wm->_actualTranslationalValue;
	_wm->_agentAbsoluteOrientation += _wm->_actualRotationalVelocity;
    
	// * recalibrate orientation within ]-180°,+180°]
    
    while ( _wm->_agentAbsoluteOrientation <= -180.0 || _wm->_agentAbsoluteOrientation > 180.0 ) // assume that it is highly unlikely that this while should loop. (depends from maximal angular velocity)
    {
        if ( _wm->_agentAbsoluteOrientation <= -180.0 )
        {
            _wm->_agentAbsoluteOrientation = _wm->_agentAbsoluteOrientation + 360.0;
        }
        else
        {
            if ( _wm->_agentAbsoluteOrientation > 180.0 )
            {
                _wm->_agentAbsoluteOrientation = _wm->_agentAbsoluteOrientation - 360.0;
            }
        }
    }
}
