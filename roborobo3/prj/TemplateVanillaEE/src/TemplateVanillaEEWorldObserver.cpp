/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "Observers/AgentObserver.h"
#include "Observers/WorldObserver.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEWorldObserver.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEController.h"
#include "World/World.h"

TemplateVanillaEEWorldObserver::TemplateVanillaEEWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
}

TemplateVanillaEEWorldObserver::~TemplateVanillaEEWorldObserver()
{
    // superclass constructor called before
}

void TemplateVanillaEEWorldObserver::step()
{
    TemplateEEWorldObserver::step();
    
    /*
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == 0 )
    {
        std::cout << "[DEBUG] new generation.\n";
    }
    */

}

void TemplateVanillaEEWorldObserver::monitorPopulation( bool localVerbose )
{
    // * monitoring: count number of active agents.
    
    int activeCount = 0;
    double sumOfFitnesses = 0;
    
    for ( int i = 0 ; i != gNumberOfRobots ; i++ )
    {
        TemplateVanillaEEController *ctl = dynamic_cast<TemplateVanillaEEController*>(gWorld->getRobot(i)->getController());
        
        if ( ctl->getWorldModel()->isAlive() == true )
        {
            activeCount++;
            sumOfFitnesses += ctl->getFitness() ;
        }
    }
    
    if ( gVerbose && localVerbose )
    {
        std::cout << "[gen:" << (gWorld->getIterations()/TemplateEESharedData::gEvaluationTime) << ";it:" << gWorld->getIterations() << ";pop:" << activeCount << ";avgFitness:" << sumOfFitnesses/activeCount << "]\n";
    }
    
    // Logging, population-level: alive
    std::string sLog = std::string("") + std::to_string(gWorld->getIterations()) + ",pop,alive," + std::to_string(activeCount) + "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
}
