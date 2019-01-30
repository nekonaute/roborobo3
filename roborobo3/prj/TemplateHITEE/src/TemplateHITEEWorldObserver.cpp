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
    
    gProperties.checkAndGetPropertyValue("transferVolume",&TemplateHITEESharedData::transferVolume,true);
    gProperties.checkAndGetPropertyValue("mutationVolume",&TemplateHITEESharedData::mutationVolume,true);
    gProperties.checkAndGetPropertyValue("maturationDelayDefaultValue",&TemplateHITEESharedData::maturationDelayDefaultValue,true);
    gProperties.checkAndGetPropertyValue("memorySlidingWindowSize",&TemplateHITEESharedData::memorySlidingWindowSize,true);
    gProperties.checkAndGetPropertyValue("coordSlidingWindowSize",&TemplateHITEESharedData::coordSlidingWindowSize,true);
    gProperties.checkAndGetPropertyValue("mutationOp",&TemplateHITEESharedData::mutationOp,true);
    gProperties.checkAndGetPropertyValue("P_mutation",&TemplateHITEESharedData::P_mutation,true);
    gProperties.checkAndGetPropertyValue("resetOnMutation",&TemplateHITEESharedData::resetOnMutation,true);
    gProperties.checkAndGetPropertyValue("strictlySuperiorSelection",&TemplateHITEESharedData::strictlySuperiorSelection,true);
    gProperties.checkAndGetPropertyValue("fitnessFunction",&TemplateHITEESharedData::fitnessFunction,true);
    
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
