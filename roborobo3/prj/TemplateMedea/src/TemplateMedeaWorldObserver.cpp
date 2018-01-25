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
    
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized.\n");
    gLitelogManager->flush();
}

TemplateMedeaWorldObserver::~TemplateMedeaWorldObserver()
{
    // superclass destructor called before
}

void TemplateMedeaWorldObserver::initPre()
{
    // nothing to do.
}

void TemplateMedeaWorldObserver::initPost()
{
    // nothing to do.
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
