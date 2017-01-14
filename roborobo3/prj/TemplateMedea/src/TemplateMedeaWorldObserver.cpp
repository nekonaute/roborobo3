/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "Observers/AgentObserver.h"
#include "Observers/WorldObserver.h"
#include "TemplateMedea/include/TemplateMedeaWorldObserver.h"
#include "TemplateMedea/include/TemplateMedeaController.h"
#include "World/World.h"

TemplateMedeaWorldObserver::TemplateMedeaWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass destructor called before
}

TemplateMedeaWorldObserver::~TemplateMedeaWorldObserver()
{
    // superclass destructor called before
}
