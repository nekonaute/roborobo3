/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "EEcreditassignement/include/EEcreditassignementWorldObserver.h"
#include "EEcreditassignement/include/EEcreditassignementController.h"
#include "EEcreditassignement/include/EEcreditassignementSharedData.h"
#include "EEcreditassignement/include/EEcreditassignementEnergyItem.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include <math.h>

EEcreditassignementWorldObserver::EEcreditassignementWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
}

EEcreditassignementWorldObserver::~EEcreditassignementWorldObserver()
{
    // superclass destructor called before
}

void EEcreditassignementWorldObserver::initPre()
{
    TemplateEEWorldObserver::initPre();
    
    int nbObjectsTotal = 150;//80;
    int nbObjectsInLeftRegion = 140;//75; // provide exact value (rather than a percentage) to avoid undesirable stochastic effect.
    
    for ( int i = 0 ; i < nbObjectsTotal ; i++ ) // 300
    {
        // * create a new (custom) object
        
        int id = PhysicalObjectFactory::getNextId();
        EEcreditassignementEnergyItem *object = new EEcreditassignementEnergyItem(id);
        gPhysicalObjects.push_back( object );
        
        if ( i < nbObjectsInLeftRegion ) // proportion in left/right parts of environment
            object->setOffset(0);
        else
            object->setOffset(0.5);
        
        object->relocate();
    }
}

void EEcreditassignementWorldObserver::initPost()
{
    TemplateEEWorldObserver::initPost();
    
    gNbOfPhysicalObjects = (int)gPhysicalObjects.size(); // must be done in the initPost() method for objects created in initPre().
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
