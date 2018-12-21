/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateHITEE/include/TemplateHITEEAgentObserver.h"
#include "TemplateHITEE/include/TemplateHITEEController.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/PhysicalObject.h"

TemplateHITEEAgentObserver::TemplateHITEEAgentObserver( RobotWorldModel *wm ) : TemplateEEAgentObserver ( wm )
{
    // superclass constructor called before
}

TemplateHITEEAgentObserver::~TemplateHITEEAgentObserver()
{
    // superclass destructor called before
}

/*
 * Manage foraging of energy items.
 * If walked on, the item disappear (callback to item object) and agent's fitness function is updated.
 * Assume that *only* energy point's footprint can be walked upon. That means that for this code to correctly run you should avoid placing any objects detectable through the robot's ground sensor.
 *
 */
void TemplateHITEEAgentObserver::stepPre()
{
    // * update fitness (if needed)
    if ( _wm->isAlive() )
    {
        if ( PhysicalObject::isInstanceOf(_wm->getGroundSensorValue()) )
        {
            dynamic_cast<TemplateHITEEController*>(getController())->updateFitness2(1);
        }
        else
        {
            dynamic_cast<TemplateHITEEController*>(getController())->updateFitness2(0);
        }
    }

    TemplateEEAgentObserver::stepPre();
}



