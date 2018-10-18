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

TutorialController::TutorialController( RobotWorldModel *__wm ) : Controller ( __wm )
{
    if ( _wm->_cameraSensorsNb != NB_SENSORS )
    {
        std::cerr << "[CRITICAL] This project assumes robot specifications with " << NB_SENSORS << " sensors (provided: " << _wm->_cameraSensorsNb << " sensors). STOP.\n";
        exit(-1);
    }
}

TutorialController::~TutorialController()
{
    // nothing to do.
}

void TutorialController::reset()
{
    // nothing to do.
}

// called at everytime step. Each robot is called once per timestep, but order of call always change from step to step. This is *the* function (and possibly the only one, in most case) you have to change.
void TutorialController::step()
{
    // QUICK HELP
    //
    // setRotation(double value) and setTranslation(double value) take values in [-1.0,+1.0]
    // rotation: positive value means clock-wise rotation.
    // note that these are *desired* rotation/translation. The robot may not be able to comply if it is stuck.
    //
    // getDistance returns a value in [0,1], with 1 as the maximal value (i.e. see nothing within (limited) eyesight)
    
    
    // * How to access various useful sensory information
    // Full list: check <roborobo3>/include/core/Controllers/Controller.h - section "Accessing methods"
    
    // Display information for agent on focus, only if focus active.
    // note that at start up, gRobotDisplayFocus is false.
    // to activate gRobotDisplayFocus, press the "F" key at runtime
    // to change the robot under focus, use <tab> (or <shift+tab>)
    // reminder: all commands available at run-time can be displayed in the console with the "H" key (=help)
    if ( gRobotDisplayFocus && getId() == gRobotIndexFocus )
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
