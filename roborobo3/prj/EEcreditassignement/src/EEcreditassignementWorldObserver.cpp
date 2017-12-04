/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "EEcreditassignement/include/EEcreditassignementWorldObserver.h"
#include "EEcreditassignement/include/EEcreditassignementController.h"
#include "EEcreditassignement/include/EEcreditassignementSharedData.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"

EEcreditassignementWorldObserver::EEcreditassignementWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
}

EEcreditassignementWorldObserver::~EEcreditassignementWorldObserver()
{
    // superclass destructor called before
}

void EEcreditassignementWorldObserver::stepPre()
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

void EEcreditassignementWorldObserver::stepPost( )
{
    TemplateEEWorldObserver::stepPost();
}

void EEcreditassignementWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}
