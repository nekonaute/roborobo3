/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateMedea/include/TemplateMedeaWorldObserver.h"
#include "World/World.h"

TemplateMedeaWorldObserver::TemplateMedeaWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
}

TemplateMedeaWorldObserver::~TemplateMedeaWorldObserver()
{
    // superclass destructor called before
}

void TemplateMedeaWorldObserver::stepPre()
{
    // nothing to do.
}

void TemplateMedeaWorldObserver::stepPost()
{
    // nothing to do.
}
