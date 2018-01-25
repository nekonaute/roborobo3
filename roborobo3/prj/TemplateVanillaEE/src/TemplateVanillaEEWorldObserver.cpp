/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateVanillaEE/include/TemplateVanillaEEWorldObserver.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEController.h"
#include "TemplateVanillaEE/include/TemplateVanillaEESharedData.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"

TemplateVanillaEEWorldObserver::TemplateVanillaEEWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
    
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized.\n");
    gLitelogManager->flush();
}

TemplateVanillaEEWorldObserver::~TemplateVanillaEEWorldObserver()
{
    // superclass destructor called before
}

void TemplateVanillaEEWorldObserver::initPre()
{
    // nothing to do.
}

void TemplateVanillaEEWorldObserver::initPost()
{
    // nothing to do.
}

void TemplateVanillaEEWorldObserver::stepPre()
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

void TemplateVanillaEEWorldObserver::stepPost( )
{
    TemplateEEWorldObserver::stepPost();
}

void TemplateVanillaEEWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}
