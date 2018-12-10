/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateHITEE/include/TemplateHITEEWorldObserver.h"
#include "TemplateHITEE/include/TemplateHITEEController.h"
#include "TemplateHITEE/include/TemplateHITEESharedData.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"

TemplateHITEEWorldObserver::TemplateHITEEWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
    
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized.\n");
    gLitelogManager->flush();
}

TemplateHITEEWorldObserver::~TemplateHITEEWorldObserver()
{
    // superclass destructor called before
}

void TemplateHITEEWorldObserver::initPre()
{
    // nothing to do.
}

void TemplateHITEEWorldObserver::initPost()
{
    // nothing to do.
}

void TemplateHITEEWorldObserver::stepPre()
{
    TemplateEEWorldObserver::stepPre();
    /*
    // EXAMPLE
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == 0 )
    {
        std::cout << "[INFO] new generation.\n";
    }
    */
}

void TemplateHITEEWorldObserver::stepPost( )
{
    TemplateEEWorldObserver::stepPost();
}

void TemplateHITEEWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}
