/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "ForagingRegions/include/ForagingRegionsWorldObserver.h"
#include "ForagingRegions/include/ForagingRegionsController.h"
#include "ForagingRegions/include/ForagingRegionsSharedData.h"
#include "ForagingRegions/include/ForagingRegionsEnergyItem.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include <math.h>

ForagingRegionsWorldObserver::ForagingRegionsWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before
}

ForagingRegionsWorldObserver::~ForagingRegionsWorldObserver()
{
    // superclass destructor called before
}

void ForagingRegionsWorldObserver::initPre()
{
    TemplateEEWorldObserver::initPre();
    
    int nbObjectsTotal = 150;//80;
    int nbObjectsInLeftRegion = 140;//75; // provide exact value (rather than a percentage) to avoid undesirable stochastic effect.
    
    for ( int i = 0 ; i < nbObjectsTotal ; i++ ) // 300
    {
        // * create a new (custom) object
        
        int id = PhysicalObjectFactory::getNextId();
        ForagingRegionsEnergyItem *object = new ForagingRegionsEnergyItem(id);
        gPhysicalObjects.push_back( object );
        
        if ( i < nbObjectsInLeftRegion ) // proportion in left/right parts of environment
            object->setOffset(0);
        else
            object->setOffset(0.5);
        
        object->relocate();
    }
}

void ForagingRegionsWorldObserver::initPost()
{
    TemplateEEWorldObserver::initPost();
    
    gNbOfPhysicalObjects = (int)gPhysicalObjects.size(); // must be done in the initPost() method for objects created in initPre().
}

void ForagingRegionsWorldObserver::stepPre()
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

void ForagingRegionsWorldObserver::stepPost( )
{
    TemplateEEWorldObserver::stepPost();
}

void ForagingRegionsWorldObserver::monitorPopulation( bool localVerbose )
{
    TemplateEEWorldObserver::monitorPopulation(localVerbose);
}
