/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "Sandbox/include/SandboxWorldObserver.h"
#include "Sandbox/include/SandboxController.h"
#include "Sandbox/include/SandboxSharedData.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"

SandboxWorldObserver::SandboxWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
    
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized.\n");
    gLitelogManager->flush();
}

SandboxWorldObserver::~SandboxWorldObserver()
{
    // superclass destructor called before
}

void SandboxWorldObserver::initPre()
{
    // nothing to do.
}

void SandboxWorldObserver::initPost()
{
    // nothing to do.
}

void SandboxWorldObserver::stepPre()
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

void SandboxWorldObserver::stepPost( )
{
    TemplateEEWorldObserver::stepPost();
}

void SandboxWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}
