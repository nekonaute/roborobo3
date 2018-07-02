/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#include "TemplateWander/include/TemplateWanderController.h"
#include "WorldModels/RobotWorldModel.h"
#include "RoboroboMain/roborobo.h"
#include "World/World.h"

// Load readable sensor names
#define NB_SENSORS 8 // should be coherent with gRobotSpecsImageFilename value read from the property file.
#include "Utilities/Sensorbelt.h"

TemplateWanderController::TemplateWanderController( RobotWorldModel *__wm ) : Controller ( __wm )
{
    if ( _wm->_cameraSensorsNb != NB_SENSORS )
    {
        std::cerr << "[CRITICAL] This project assumes robot specifications with " << NB_SENSORS << " sensors (provided: " << _wm->_cameraSensorsNb << " sensors). STOP.\n";
        exit(-1);
    }
}

TemplateWanderController::~TemplateWanderController()
{
	// nothing to do.
}

void TemplateWanderController::reset()
{
	// nothing to do.
}

// a basic obstacle avoidance behavior
void TemplateWanderController::step()
{
    // QUICK HELP
    //
    // setRotation(double value) and setTranslation(double value) take values in [-1.0,+1.0]
    // rotation: positive value means clock-wise rotation.
    // note that these are *desired* rotation/translation. The robot may not be able to comply if it is stuck.
    //
    // getDistance returns a value in [0,1], with 1 as the maximal value (i.e. see nothing within (limited) eyesight)
    
    setTranslation( sin( ( ( getDistance(SENSOR_FFL) + getDistance(SENSOR_FFR) ) / 2.0 )* M_PI / 2.0) ); //( getDistance(SENSOR_FFL) + getDistance(SENSOR_FFR) ) / 2.0 );

    double distanceOnMyLeft = getDistance(SENSOR_L) + getDistance(SENSOR_FL) + getDistance(SENSOR_FFL);
    double distanceOnMyRight = getDistance(SENSOR_R) + getDistance(SENSOR_FR) + getDistance(SENSOR_FFR);

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
