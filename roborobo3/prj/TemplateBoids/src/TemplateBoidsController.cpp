/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */

#include "TemplateBoids/include/TemplateBoidsController.h"
#include "TemplateBoids/include/TemplateBoidsWorldObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include <math.h>
#include <string>


using namespace Neural;

TemplateBoidsController::TemplateBoidsController( RobotWorldModel *wm )
{
    _wm = wm;
    
    _iteration = 0;
    
    _wm->updateLandmarkSensor();
    
    _wm->setAlive(true);
    _wm->setRobotLED_colorValues(255, 0, 0);
}

TemplateBoidsController::~TemplateBoidsController()
{
}

void TemplateBoidsController::reset()
{
}

void TemplateBoidsController::step()
{
    _iteration++;
    
    // *
    // * Parameter settings for the boid controller
    // *
    // *    Behaviors are ordered by priority, as follows:
    // *        if distance-to-wall < wallAvoidanceMinDist => triggers wall avoidance
    // *        if distance-to-center-of-mass < repulsiveRadius  => triggers repulsion
    // *        if distance-to-center-of-mass > attractionRadius => triggers attraction
    // *        if distance-to-center-of-mass in ]repulsiveRadius,attractionRadius[ => triggers orientation
    // *        if nobody around => move forward
    // *
    // *    All values are set in [0,1[
    // *
    
    double wallAvoidanceMinDist = gSensorRange*0.5;
    double repulsiveRadius = gSensorRange*0.1;
    double attractionRadius = gSensorRange*0.8; // 0.7 // if ( attractionRadius < repulsiveRadius ) then: assume attractionRadius=repulsiveRadius
    double orientationDelta = 0.0; // normalized in [0,1[ // 0.0
    
    
    // *
    // * get information from surroundings
    // *
    
    double avgDeltaOrientation = 0;
    double avgCenterOfMass_X = 0;
    double avgCenterOfMass_Y = 0;
    
    double minDist = gSensorRange+1;
    // double closestRobotDeltaOrientation = 0; // unused
    
    int nbNeighbours = 0;
    
    double minDistToWall = gSensorRange*2;
    
    for( int i = 0 ; i < _wm->_cameraSensorsNb ; i++)
    {
        int targetRawIndex = _wm->getObjectIdFromCameraSensor(i);
        
        if ( targetRawIndex >= gRobotIndexStartOffset )   // sensor ray bumped into a robot -- WARNING: one (very close) neighbor can be seen by more than one sensor (in the following, we dont care if this happen, though it is not correct to do so)
        {
            int targetRobotIndex = targetRawIndex - gRobotIndexStartOffset; // convert image registering index into robot id.
            
            nbNeighbours++;
            
            // update average center of mass
            
            avgCenterOfMass_X = avgCenterOfMass_X + gWorld->getRobot(targetRobotIndex)->getWorldModel()->_xReal;
            avgCenterOfMass_Y = avgCenterOfMass_Y + gWorld->getRobot(targetRobotIndex)->getWorldModel()->_yReal;
            
            // update delta orientation
            
            double srcOrientation = _wm->_agentAbsoluteOrientation;
            double tgtOrientation = gWorld->getRobot(targetRobotIndex)->getWorldModel()->_agentAbsoluteOrientation;
            
            double delta_orientation = srcOrientation - tgtOrientation;
            if ( delta_orientation >= 180.0 )
                delta_orientation = - ( 360.0 - delta_orientation );
            else
                if ( delta_orientation <= -180.0 )
                    delta_orientation = - ( - 360.0 - delta_orientation );
            
            delta_orientation = delta_orientation/180.0;
            
            avgDeltaOrientation += delta_orientation;
            
            // update min/max distance (if relevant)
            
            double dist = _wm->getCameraSensorValue(i,SENSOR_DISTANCEVALUE);
            
            if ( dist < minDist )
            {
                minDist = dist;
                //closestRobotDeltaOrientation = delta_orientation; // unused
            }
        }
        else
        {
            if ( targetRawIndex == 0 && _wm->getCameraSensorValue(i,SENSOR_DISTANCEVALUE) < minDistToWall ) // closest wall
                minDistToWall = _wm->getCameraSensorValue(i,SENSOR_DISTANCEVALUE);
        }
    }
    
    avgDeltaOrientation = avgDeltaOrientation / nbNeighbours ;
    avgCenterOfMass_X = avgCenterOfMass_X / nbNeighbours;
    avgCenterOfMass_Y = avgCenterOfMass_Y / nbNeighbours;
    
    
    // *
    // * Decision making
    // *
    
    _wm->_desiredRotationalVelocity = 0;
    
    if ( minDistToWall < wallAvoidanceMinDist ) // (too) close to a wall?
    {
        // * a basic obstacle avoidance behavior
        
        if ( ( (double)(rand()%100))/100.0 < 0.2 )
        {
            // pure random move
            _wm->_desiredRotationalVelocity = (double)((rand()%360)-180);
        }
        else
        {
            // obstacle avoidance (assume 8 sensors, but works ok with 12)
            _wm->_desiredTranslationalValue =  + 1 - ( (double)gSensorRange - ((_wm->getCameraSensorValue(2,SENSOR_DISTANCEVALUE)+_wm->getCameraSensorValue(3,SENSOR_DISTANCEVALUE))/2) )  / (double)gSensorRange;
            if ( _wm->getCameraSensorValue(0,SENSOR_DISTANCEVALUE) + _wm->getCameraSensorValue(1,SENSOR_DISTANCEVALUE) + _wm->getCameraSensorValue(2,SENSOR_DISTANCEVALUE) < _wm->getCameraSensorValue(3,SENSOR_DISTANCEVALUE) + _wm->getCameraSensorValue(4,SENSOR_DISTANCEVALUE) + _wm->getCameraSensorValue(5,SENSOR_DISTANCEVALUE) )
                _wm->_desiredRotationalVelocity = +5;
            else
                if ( _wm->getCameraSensorValue(3,SENSOR_DISTANCEVALUE) + _wm->getCameraSensorValue(4,SENSOR_DISTANCEVALUE) + _wm->getCameraSensorValue(5,SENSOR_DISTANCEVALUE) < 3*gSensorRange )
                    _wm->_desiredRotationalVelocity = -5;
                else
                    if ( _wm->_desiredRotationalVelocity > 0 )
                        _wm->_desiredRotationalVelocity--;
                    else
                        if ( _wm->_desiredRotationalVelocity < 0)
                            _wm->_desiredRotationalVelocity++;
                        else
                            _wm->_desiredRotationalVelocity = 0.01 - (double)(rand()%10)/10.*0.02;
        }
    }
    else
    {
        // * boids controller
        
        if ( nbNeighbours == 0 )
        {
            // nobody? go forward.
            _wm->_desiredRotationalVelocity = 0;
        }
        else
        {
            if ( minDist < repulsiveRadius )
            {
                // (1) repulsion
                
                double angleDeltaToCenterOfMass = getAngleToTarget( _wm->_xReal, _wm->_yReal, _wm->_agentAbsoluteOrientation, avgCenterOfMass_X,avgCenterOfMass_Y);
                
                //if ( closestRobotDeltaOrientation < 0 ) // alternate method. Not as efficient.
                if ( angleDeltaToCenterOfMass < 0 )
                    _wm->_desiredRotationalVelocity = +10;
                else
                    _wm->_desiredRotationalVelocity = -10;
            }
            else
            {
                if ( minDist > attractionRadius )
                {
                    // (3) attraction
                    
                    double angleDeltaToCenterOfMass = getAngleToTarget( _wm->_xReal, _wm->_yReal, _wm->_agentAbsoluteOrientation, avgCenterOfMass_X,avgCenterOfMass_Y);
                    
                    if ( angleDeltaToCenterOfMass < 0 )
                        _wm->_desiredRotationalVelocity = -10;
                    else
                        _wm->_desiredRotationalVelocity = +10;
                }
                else
                {
                    
                    if ( fabs(avgDeltaOrientation) > orientationDelta   )
                    {
                        // (2) orientation
                        if ( avgDeltaOrientation < 0 )
                            _wm->_desiredRotationalVelocity = +10;
                        else
                            _wm->_desiredRotationalVelocity = -10;
                    }
                    else
                    {
                        // default: nothing to do? go forward.
                        _wm->_desiredRotationalVelocity = 0;
                    }
                }
            }
        }
        
        _wm->_desiredTranslationalValue = 3;
    }
}

/*
void TemplateBoidsController::step() // THIS IS A TEMPLATE FOR PRACTICAL APPLICATION (moving like an idiot , gathering sensory information for nothing)
{
    _iteration++;
    
    // *
    // * Parameter settings for the boid controller
    // *
    // *    Behaviors are ordered by priority, as follows:
    // *        if distance-to-wall < wallAvoidanceMinDist => triggers wall avoidance
    // *        if distance-to-center-of-mass < repulsiveRadius  => triggers repulsion
    // *        if distance-to-center-of-mass > attractionRadius => triggers attraction
    // *        if distance-to-center-of-mass in ]repulsiveRadius,attractionRadius[ => triggers orientation
    // *        if nobody around => move forward
    // *
    // *    All values are set in [0,1[
    // *
    
    double wallAvoidanceMinDist = gSensorRange*0.1;
    double repulsiveRadius = gSensorRange*0.1;
    double attractionRadius = gSensorRange*0.8;
    double orientationDelta = 0.0;
    
    
    // *
    // * get random information from surroundings!
    // *
    
    // Get distance to closest obstacle (wall or robot) from 8 sensors around the robot (works with the 12-sensors robot)
    double leftSensor = _wm->getCameraSensorValue(0,SENSOR_DISTANCEVALUE);
    double leftFwdSensor = _wm->getCameraSensorValue(2,SENSOR_DISTANCEVALUE);
    double fwdSensor = _wm->getCameraSensorValue(4,SENSOR_DISTANCEVALUE);
    double rightFwdSensor = _wm->getCameraSensorValue(6,SENSOR_DISTANCEVALUE);
    double rightSensor = _wm->getCameraSensorValue(8,SENSOR_DISTANCEVALUE);
    double rightBwdSensor = _wm->getCameraSensorValue(9,SENSOR_DISTANCEVALUE);
    double bwdSensor = _wm->getCameraSensorValue(10,SENSOR_DISTANCEVALUE);
    double leftBwdSensor = _wm->getCameraSensorValue(11,SENSOR_DISTANCEVALUE);

    int nbNeighbors = 0;
    
    for( int i = 0 ; i < _wm->_cameraSensorsNb ; i++)
    {
        int targetRawIndex = _wm->getObjectIdFromCameraSensor(i);
        
        if ( targetRawIndex >= gRobotIndexStartOffset )   // sensor ray bumped into a robot
        {
            int targetRobotIndex = targetRawIndex - gRobotIndexStartOffset; // convert image registering index into robot id.
            
            nbNeighbors++;
            
            double xMyself = _wm->_xReal;
            double yMyself = _wm->_yReal;
            double alphaMyself = _wm->_agentAbsoluteOrientation;
            
            double xNeighbor = gWorld->getRobot(targetRobotIndex)->getWorldModel()->_xReal;
            double yNeighbor = gWorld->getRobot(targetRobotIndex)->getWorldModel()->_yReal;
            double alphaNeighbor = gWorld->getRobot(targetRobotIndex)->getWorldModel()->_agentAbsoluteOrientation;

            double angleDeltaToNeighbor = getAngleToTarget(xMyself, yMyself, alphaMyself, xNeighbor, yNeighbor);

            std::cout << "robot no." << _wm->_id << " (x=" << xMyself << ",y=" << yMyself << "," << alphaMyself << "°) sees robot no." << targetRobotIndex << " (x=" << xNeighbor << ",y=" << yNeighbor << "," << alphaNeighbor << "°) at distance " << _wm->getCameraSensorValue(i,SENSOR_DISTANCEVALUE) << ", angle " << angleDeltaToNeighbor << "°!\n";
            // note: here, angle means at which angle does the robot sees its neighbor wrt to the direction it is facing.
            
            double delta_orientation = alphaMyself - alphaNeighbor;
            if ( delta_orientation >= 180.0 )
                delta_orientation = - ( 360.0 - delta_orientation );
            else
                if ( delta_orientation <= -180.0 )
                    delta_orientation = - ( - 360.0 - delta_orientation );
            delta_orientation = delta_orientation/180.0;
        }
        else
        {
            if ( targetRawIndex == 0 && _wm->getCameraSensorValue(i,SENSOR_DISTANCEVALUE) < 10 ) // closest wall
            {
                std::cout << "robot no." << _wm->_id << " sees a wall within distance=10 (distance: " << _wm->getCameraSensorValue(i,SENSOR_DISTANCEVALUE) << ")!\n";
            }
        }
    }

    // *
    // * move like an idiot!
    // *
    
    _wm->_desiredRotationalVelocity = 2 + (double)(rand()%10)/10.*0.02;
    _wm->_desiredTranslationalValue = 3;
    
}
*/