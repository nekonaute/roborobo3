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
    
    int border = 40;
    
    for ( int i = 0 ; i < 1000 ; i++ )
    {
        // * create a new (custom) object
        
        int id = PhysicalObjectFactory::getNextId();
        EEcreditassignementEnergyItem *object = new EEcreditassignementEnergyItem(id);
        gPhysicalObjects.push_back( object );
        
        // * pick new coordinate
        
        object->unregisterObject();

        double pi = atan(1)*4;
        
        do{
            
            double xPos;
        
            //double value = random();
            //xPos = (1/(1 + std::exp(-value*5 + 13)))*3000; // (1/(1 + Exp[-x*5 + 13])*3000) <===
            //xPos = std::pow(value,3); // x^3   <==
            //xPos = (1/(1 + std::exp(-value*5 + 5)))*2; // (1/(1 + Exp[-x*5 + 5]))
            //xPos = 1 - ( 1 / (1 + std::exp( -value * 10 + 5))); // 1 - (1/(1 + Exp[-x*10 + 5]))
        
            double sigma = 0.2;
            double gaussianPeakValue = 1.0 / std::sqrt( 2. * pi * std::pow(sigma,2) );
            xPos = sigma*randgaussian() / gaussianPeakValue;
            
            double x = xPos * ( gScreenWidth - 2*border ) + border;
            double y = random() * ( gScreenHeight - 2*border ) + border;
        
            object->setCoordinates(x,y);
        
        } while ( object->canRegister() == false );
        
        object->registerObject();
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
