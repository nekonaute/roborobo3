/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 * 2018-10-18
 */


#include "Tutorial/include/TutorialController.h"
#include "WorldModels/RobotWorldModel.h"
#include "RoboroboMain/roborobo.h"
#include "World/World.h"

// Load readable sensor names
#define NB_SENSORS 12 // should be coherent with gRobotSpecsImageFilename value read from the property file.
#include "Utilities/Sensorbelt.h"

/* **** **** **** */
/* **** **** **** */
/* **** **** **** */

TutorialController::TutorialController( RobotWorldModel *__wm ) : Controller ( __wm )
{
    if ( _wm->_cameraSensorsNb != NB_SENSORS )
    {
        std::cerr << "[CRITICAL] This project assumes robot specifications with " << NB_SENSORS << " sensors (provided: " << _wm->_cameraSensorsNb << " sensors). STOP.\n";
        exit(-1);
    }
}

/* **** **** **** */
/* **** **** **** */
/* **** **** **** */

TutorialController::~TutorialController()
{
    // nothing to do.
}

/* **** **** **** */
/* **** **** **** */
/* **** **** **** */

void TutorialController::reset()
{
    // nothing to do.
}

/* **** **** **** */
/* **** **** **** */
/* **** **** **** */

// called at everytime step. Each robot is called once per timestep, but order of call always change from step to step. This is *the* function (and possibly the only one, in most case) you have to change.
void TutorialController::step()
{
    // QUICK HELP
    //
    //
    // ## SENSORS ##
    //
    // getDistance returns a real value in [0,1], with 1 as the maximal value (i.e. see nothing within (limited) eyesight)
    //      To specify which sensor is requested, it is possible to use constant defines in include/core/Utilities/Sensorbelt.h
    //      Depending on the number of sensors, the following sensor "names" can be used:
    //          8 sensors:
    //          12 sensors:
    //          16 sensors:
    //
    // getActualRotation returns a real value in [-1,1] which represents the current rotational speed
    // getActualTranslation returns a real value in [-1,1] which represents the current translational speed
    // getOrientation returns a real value in [-1,1], which represents an absolute orientation wrt the center-top of the arena
    //
    // ## ACTUATORS ##
    //
    // setRotation(double value) and setTranslation(double value) take values in [-1.0,+1.0]
    //      rotation: positive value means clock-wise rotation.
    //      note that these are *desired* rotation/translation (see below). The robot may not be able to comply if it is stuck.
    //
    // The actual maximal translation speed, maximal rotational speed and maximal rotational update values are defined in the related properties file. Relevant parameters are:
    //      gMaxTranslationalSpeed
    //          strictly positive integer value.
    //          it is strongly advised *not* to assign a value greated than the diameter of the robot to avoid jump in space.
    //          suggested value: 3, for mini-robots
    //          Values passed to the setTranslation function are mapped to [-gMaxTranslationalSpeed,+gMaxTranslationalSpeed]
    //      gMaxRotationalSpeed
    //          strictly positive integer value, in ]0,180]
    //          expressed in degrees per step.
    //          suggested value: 30
    //      gMaxTranslationalDeltaValue
    //          strictly positive integer value, in ]0,gMaxRotationalSpeed]
    //          Maximum angle update per step for rotational speed
    //          Rationale: this models the limits of change the motor output during one time step.
    //          gMaxTranslationalDeltaValue and gMaxRotationalSpeed can have a similar value, i.e. motor is over-powered.
    //          Values passed to the setRotational function are
    //              1. mapped to [-gMaxTranslationalDeltaValue,+gMaxTranslationalDeltaValue]
    //              2. actual rotational speed is updated using the value from (1), ie. increased, decreased or unchanged.
    //              3. if needed, actual rotational speed is bounded to fit within [-gMaxRotationalSpeed,+gMaxRotationalSpeed]
    
    // * How to access various useful sensory information
    // Full list: check <roborobo3>/include/core/Controllers/Controller.h - section "Accessing methods"
    
    // Display information for agent on focus, only if focus active.
    // note that at start up, gRobotDisplayFocus is false.
    // to activate gRobotDisplayFocus, press the "F" key at runtime
    // to change the robot under focus, use <tab> (or <shift+tab>)
    // reminder: all commands available at run-time can be displayed in the console with the "H" key (=help)
    
    if ( gRobotDisplayFocus && getId() == gRobotIndexFocus ) // Execute __only__ for robot under focus (press "F" to focus on a robot, <tab> and <shift>+<tab> to change robot focus)
    {
        std::cout << "Robot #" << getId() << "\n";
        std::cout << "\torientation: " << getOrientation()*180.0 << "°\n";
        Point2d pos = getPosition();
        std::cout << "\tposition: ( " << pos.x << " , " << pos.y << " )\n";
        std::cout << "\tactual translation: " << getActualTranslation() << " (in [-1,+1] wrt min/max values)\n";
        std::cout << "\tactual rotation: " << getActualRotation() << " (in [-1,+1] wrt min/max values)\n";
        
        std::cout << "\tsensorbelt status:" << "\n";
        for ( int i = 0 ; i != NB_SENSORS ; i++ ) // also possible to check each sensor separatly using SENSOR_L, SENSOR_R, etc. (check Utilities/Sensorbelt.h)
        {
            double dist = getDistanceAt(i);
            std::cout << "\t\t[" << i << "] ";
            if ( getWallAt(i) == 1 )
            {
                std::cout << "[dist=" << dist << "] Wall\n";
            }
            else
            {
                int robotId = getRobotIdAt(i);
                if ( robotId != -1 )
                {
                    std::cout << "[dist=" << dist << "] Robot #" << robotId << "\n\t\t      relative orientation wrt robot "<<getId()<<" : " << getRobotRelativeOrientationAt( i )*180.0 << "°\n";
                    
                    // accessing the target robot's controller
                    // can be useful if additional methods have been implemented (e.g. communication)
                    TutorialController* targetRobotController = dynamic_cast<TutorialController*>(getRobotControllerAt(i));
                    std::cout << "\t\t      [double-check] target robot's id really is #" << targetRobotController->getId() << "\n"; // example of use
                    
                    sendMessage(targetRobotController,"Ping");
                    
                }
                else
                {
                    std::cout << "[dist=" << dist << "] Nothing\n";
                }
            }
        }
    }
    
    // * How to use sensors to drive the robot
    // This is an example that code for wall avoidance
    
    setTranslation( sin( ( ( getDistanceAt(SENSOR_FFL) + getDistanceAt(SENSOR_FFR) ) / 2.0 )* M_PI / 2.0) );
    
    double distanceOnMyLeft = getDistanceAt(SENSOR_L) + getDistanceAt(SENSOR_FL) + getDistanceAt(SENSOR_FFL);
    double distanceOnMyRight = getDistanceAt(SENSOR_R) + getDistanceAt(SENSOR_FR) + getDistanceAt(SENSOR_FFR);
    
    double rotationDelta = 0.3;
    double noiseAmplitude = 0.01;
    
    if ( distanceOnMyLeft < distanceOnMyRight )
        setRotation( +rotationDelta );
    else
        if ( distanceOnMyRight < distanceOnMyLeft )
            setRotation( -rotationDelta );
        else
            //setRotation( 0.1 - (double)(random01()*0.2));
            setRotation( noiseAmplitude * ( 1.0 - (double)(random01()*2.0) ) );
    
}

/* **** **** **** */
/* **** **** **** */
/* **** **** **** */

void TutorialController::sendMessage(TutorialController* _targetRobot, std::string _message)
{
    _targetRobot->receiveMessage(getId(),_message);
}

/* **** **** **** */
/* **** **** **** */
/* **** **** **** */

void TutorialController::receiveMessage(int _senderId, std::string _message)
{
    std::cout << "\t\t\t>>> [Robot #" << getId() << "] received message from robot #" << _senderId << " : \"" << _message << "\" <<<\n";
}

/* **** **** **** */
/* **** **** **** */
/* **** **** **** */
