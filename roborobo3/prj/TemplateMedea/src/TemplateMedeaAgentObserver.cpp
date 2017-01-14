/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#include "TemplateMedea/include/TemplateMedeaAgentObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include "RoboroboMain/roborobo.h"
#include "TemplateMedea/include/TemplateMedeaController.h"
#include <cmath>
#include "TemplateMedea/include/TemplateMedeaWorldObserver.h"
#include <string>

TemplateMedeaAgentObserver::TemplateMedeaAgentObserver( RobotWorldModel *wm ) : TemplateEEAgentObserver ( wm )
{
    // superclass constructor called before
}

TemplateMedeaAgentObserver::~TemplateMedeaAgentObserver()
{
    // superclass destructor called before
}
