/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateMedea/include/TemplateMedeaAgentObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "RoboroboMain/roborobo.h"

TemplateMedeaAgentObserver::TemplateMedeaAgentObserver( RobotWorldModel *wm ) : TemplateEEAgentObserver ( wm )
{
    // superclass constructor called before
}

TemplateMedeaAgentObserver::~TemplateMedeaAgentObserver()
{
    // superclass destructor called before
}

/*
 * Manage energy wrt. foraging (if energy management is activated in parameter files)
 *
 */
void TemplateMedeaAgentObserver::stepPre()
{
    // * update energy if needed
    if ( gEnergyLevel && _wm->isAlive() )
    {
        _wm->substractEnergy(1);
        assert( _wm->getEnergyLevel() >= 0 );
        if ( _wm->getEnergyLevel() == 0 )
            _wm->setAlive(false);
    }
        
    TemplateEEAgentObserver::stepPre();
}
