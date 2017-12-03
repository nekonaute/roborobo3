/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateMedea/include/TemplateMedeaWorldObserver.h"
#include "TemplateMedea/include/TemplateMedeaController.h"
#include "TemplateMedea/include/TemplateMedeaSharedData.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"
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
    TemplateEEWorldObserver::stepPre();
}

void TemplateMedeaWorldObserver::stepPost()
{
    TemplateEEWorldObserver::stepPost();
}

void TemplateMedeaWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}
