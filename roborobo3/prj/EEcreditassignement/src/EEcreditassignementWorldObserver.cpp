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

void EEcreditassignementWorldObserver::reset()
{
    TemplateEEWorldObserver::reset();
    
    int border = 40;

    for ( int i = 0 ; i < gNbOfPhysicalObjects ; i++ )
        gPhysicalObjects[i]->unregisterObject();
    
    for ( int i = 0 ; i < gNbOfPhysicalObjects ; i++ )
    {
        double value = random();
        double xPos;
        //xPos = std::pow(value,3); // x^3   <====
        xPos = std::pow(value,10); // x^10
        //xPos = 1 - ( 1 / (1 + std::exp( -value * 10 + 5))); // 1 - (1/(1 + Exp[-x*10 + 5]))
        gPhysicalObjects[i]->setCoordinates(xPos * ( gScreenWidth - 2*border ) + border, random() * ( gScreenHeight - 2*border ) + border );
        gPhysicalObjects[i]->registerObject();
    }
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
