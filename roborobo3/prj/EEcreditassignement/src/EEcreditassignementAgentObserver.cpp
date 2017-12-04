/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "EEcreditassignement/include/EEcreditassignementAgentObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/PhysicalObject.h"

EEcreditassignementAgentObserver::EEcreditassignementAgentObserver( RobotWorldModel *wm ) : TemplateEEAgentObserver ( wm )
{
    // superclass constructor called before
}

EEcreditassignementAgentObserver::~EEcreditassignementAgentObserver()
{
    // superclass destructor called before
}

/*
 * Manage foraging of energy items.
 * If walked on, the item disappear (callback to item object) and agent's fitness function is updated.
 * Assume that *only* energy point's footprint can be walked upon. That means that for this code to correctly run you should avoid placing any objects detectable through the robot's ground sensor.
 *
 */
void EEcreditassignementAgentObserver::step()
{
    // * update fitness (if needed)
    if ( _wm->isAlive() && PhysicalObject::isInstanceOf(_wm->getGroundSensorValue()) )
    {
        _wm->_fitnessValue = _wm->_fitnessValue + 1;
    }

    TemplateEEAgentObserver::step();
}



