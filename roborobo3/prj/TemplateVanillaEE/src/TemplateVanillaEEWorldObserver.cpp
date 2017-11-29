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
}

TemplateVanillaEEWorldObserver::~TemplateVanillaEEWorldObserver()
{
    // superclass destructor called before
}

void TemplateVanillaEEWorldObserver::stepPre()
{
    TemplateEEWorldObserver::stepPre();
    
    /*
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == 0 )
    {
        std::cout << "[DEBUG] new generation.\n";
    }
    */
}

void TemplateVanillaEEWorldObserver::stepPost( )
{
    // nothing to do.
}

void TemplateVanillaEEWorldObserver::monitorPopulation( bool localVerbose )
{
    // * monitoring: count number of active agents.
    
    int activeCount = 0;
    double sumOfFitnesses = 0;
    double minFitness = DBL_MAX;
    double maxFitness = -DBL_MAX;
    
    for ( int i = 0 ; i != gNbOfRobots ; i++ )
    {
        TemplateVanillaEEController *ctl = dynamic_cast<TemplateVanillaEEController*>(gWorld->getRobot(i)->getController());
        
        if ( ctl->getWorldModel()->isAlive() == true )
        {
            activeCount++;
            sumOfFitnesses += ctl->getFitness() ;
            if ( ctl->getFitness() < minFitness )
                minFitness = ctl->getFitness();
            if ( ctl->getFitness() > maxFitness )
                maxFitness = ctl->getFitness();
        }
    }
    
    if ( gVerbose && localVerbose )
    {
        std::cout << "[gen:" << (gWorld->getIterations()/TemplateEESharedData::gEvaluationTime) << ";it:" << gWorld->getIterations() << ";pop:" << activeCount << ";avgFitness:" << sumOfFitnesses/activeCount << "]\n";
    }
    
    // display lightweight logs for easy-parsing
    std::cout << "log," << (gWorld->getIterations()/TemplateEESharedData::gEvaluationTime) << "," << gWorld->getIterations() << "," << activeCount << "," << minFitness << "," << maxFitness << "," << sumOfFitnesses/activeCount << "\n";

    // Logging, population-level: alive
    std::string sLog = std::string("") + std::to_string(gWorld->getIterations()) + ",pop,alive," + std::to_string(activeCount) + "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
}
