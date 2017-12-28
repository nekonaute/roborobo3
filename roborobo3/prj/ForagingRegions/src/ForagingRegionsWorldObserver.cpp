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

    gProperties.checkAndGetPropertyValue("gNbObjectsOnLeft",&ForagingRegionsSharedData::nbObjectsOnLeft,true);
    gProperties.checkAndGetPropertyValue("gNbObjectsOnRight",&ForagingRegionsSharedData::nbObjectsOnRight,true);
    gProperties.checkAndGetPropertyValue("gForagingTask",&ForagingRegionsSharedData::foragingTask,true);
}

ForagingRegionsWorldObserver::~ForagingRegionsWorldObserver()
{
    // superclass destructor called before
}

void ForagingRegionsWorldObserver::initPre()
{
    TemplateEEWorldObserver::initPre();
    
    int nbObjectsTotal = ForagingRegionsSharedData::nbObjectsOnLeft + ForagingRegionsSharedData::nbObjectsOnRight;
    int nbObjectsInLeftRegion = ForagingRegionsSharedData::nbObjectsOnLeft;
    int threshold = ( ForagingRegionsSharedData::nbObjectsOnLeft + ForagingRegionsSharedData::nbObjectsOnRight ) / 2 ;

    for ( int i = 0 ; i < nbObjectsTotal ; i++ )
    {
        // * create a new (custom) object
        
        int id = PhysicalObjectFactory::getNextId();
        ForagingRegionsEnergyItem *object = new ForagingRegionsEnergyItem(id);
        gPhysicalObjects.push_back( object );
        
        switch ( ForagingRegionsSharedData::foragingTask )
        {
            case 0:
            {
                if ( i < nbObjectsInLeftRegion ) // proportion in left/right parts of environment
                    object->setRegion(0,0.5); // left part of the arena
                else
                    object->setRegion(0.5,0.5); // right part of the arena
            }
            break;
            
            case 1:
            {
                object->setRegion(0,1); // whole arena
                if ( i <= threshold )
                {
                    object->setDisplayColor(255,128,64);
                    object->setType(0);
                }
                else
                {
                    object->setDisplayColor(64,192,255);
                    object->setType(1);
                }
            }
            break;
            
            default:
            std::cerr << "[ERROR] gForagingTask value is unkown. Exiting.\n";
            break;
        }
        
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
