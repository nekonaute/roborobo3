/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#include "TemplateVanillaEE/include/TemplateVanillaEEAgentObserver.h"
#include "World/World.h"
#include "Utilities/Misc.h"
#include "RoboroboMain/roborobo.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEController.h"
#include <cmath>
#include "TemplateVanillaEE/include/TemplateVanillaEEWorldObserver.h"
#include <string>

TemplateVanillaEEAgentObserver::TemplateVanillaEEAgentObserver( RobotWorldModel *wm ) : TemplateEEAgentObserver ( wm )
{
    // superclass constructor called before
}

TemplateVanillaEEAgentObserver::~TemplateVanillaEEAgentObserver()
{
    // superclass destructor called before
}

/*
 * Manage foraging of energy items.
 * If walked on, the item disappear (callback to item object) and agent's fitness function is updated.
 * Assume that *only* energy point's footprint can be walked upon. That means that for this code to correctly run you should avoid placing any objects detectable through the robot's ground sensor.
 *
 */
void TemplateVanillaEEAgentObserver::step()
{
    // * update fitness (if needed)
    if ( _wm->isAlive() && PhysicalObject::isInstanceOf(_wm->getGroundSensorValue()) )
    {
        _wm->_fitnessValue = _wm->_fitnessValue + 1;
    }

    TemplateEEAgentObserver::step();
}
