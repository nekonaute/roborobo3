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

    gProperties.checkAndGetPropertyValue("gNbObjectsType1",&ForagingRegionsSharedData::nbObjectsType1,true);
    gProperties.checkAndGetPropertyValue("gNbObjectsType2",&ForagingRegionsSharedData::nbObjectsType2,true);
    gProperties.checkAndGetPropertyValue("gForagingTask",&ForagingRegionsSharedData::foragingTask,true);
    
    
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitnessNormalized");
    if ( ForagingRegionsSharedData::foragingTask == 0 )
        gLitelogManager->write(",#objectsLeftRegion_foraged,variance,#objectsRightRegion_foraged,variance,sumOfFitnesses,foragingBalance,avg_countForagedItemType0, stddev_countForagedItemType0, avg_countForagedItemType1, stddev_countForagedItemType1.");
    else
        gLitelogManager->write(",#objectsType0_foraged,variance,#objectsType1_foraged,variance,sumOfFitnesses,foragingBalance,avg_countForagedItemType0, stddev_countForagedItemType0, avg_countForagedItemType1, stddev_countForagedItemType1.");
    
    gLitelogManager->write(".\n");
    gLitelogManager->flush();
}

ForagingRegionsWorldObserver::~ForagingRegionsWorldObserver()
{
    // superclass destructor called before
}

void ForagingRegionsWorldObserver::initPre()
{
    TemplateEEWorldObserver::initPre();
    
    int nbObjectsTotal = ForagingRegionsSharedData::nbObjectsType1 + ForagingRegionsSharedData::nbObjectsType2;
    int nbObjectsInLeftRegion = ForagingRegionsSharedData::nbObjectsType1;
    int threshold = ForagingRegionsSharedData::nbObjectsType1;

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
                if ( i < threshold )
                {
                    object->setDisplayColor(255,128,64); // orange
                    object->setType(0);
                }
                else
                {
                    object->setDisplayColor(64,192,255); // blue
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
    //TemplateEEWorldObserver::monitorPopulation(localVerbose);
    
    // * monitoring: count number of active agents.
    
    int activeCount = 0;
    double sumOfFitnesses = 0;
    double minFitness = DBL_MAX;
    double maxFitness = -DBL_MAX;
    
    int countForagedItemType0 = 0;
    int countForagedItemType1 = 0;
    
    for ( int i = 0 ; i != gNbOfRobots ; i++ )
    {
        ForagingRegionsController *ctl = dynamic_cast<ForagingRegionsController*>(gWorld->getRobot(i)->getController());
        
        if ( ctl->getWorldModel()->isAlive() == true )
        {
            activeCount++;
            sumOfFitnesses += ctl->getFitness() ;
            if ( ctl->getFitness() < minFitness )
                minFitness = ctl->getFitness();
            if ( ctl->getFitness() > maxFitness )
                maxFitness = ctl->getFitness();
            
            countForagedItemType0 += ctl->nbForagedItemType0;
            countForagedItemType1 += ctl->nbForagedItemType1;
        }
    }
    
    double foragingBalance = std::max( countForagedItemType0 , countForagedItemType1 ) / std::min( countForagedItemType0 , countForagedItemType1 );
    
    double avgFitnessNormalized;
    
    if ( activeCount == 0 ) // arbitrary convention: in case of extinction, min/max/avg fitness values are -1
    {
        minFitness = -1;
        maxFitness = -1;
        avgFitnessNormalized = -1;
    }
    else
        avgFitnessNormalized = sumOfFitnesses/activeCount;
    
    if ( gVerbose && localVerbose )
    {
        std::cout << "[ gen:" << (gWorld->getIterations()/TemplateEESharedData::gEvaluationTime) << "\tit:" << gWorld->getIterations() << "\tpop:" << activeCount << "\tminFitness:" << minFitness << "\tmaxFitness:" << maxFitness << "\tavgFitnessNormalized:" << avgFitnessNormalized << " ]\n";
    }
    
    // display lightweight logs for easy-parsing
    std::string sLitelog =
    "log:"
    + std::to_string(gWorld->getIterations()/TemplateEESharedData::gEvaluationTime)
    + ","
    + std::to_string(gWorld->getIterations())
    + ","
    + std::to_string(activeCount)
    + ","
    + std::to_string(minFitness)
    + ","
    + std::to_string(maxFitness)
    + ","
    + std::to_string(avgFitnessNormalized)
    + ","
    + std::to_string(sumOfFitnesses)
    + ","
    + std::to_string(foragingBalance);
    
    
    //if ( ForagingRegionsSharedData::foragingTask == 1 )
    {
        double avg_countForagedItemType0 = (double)countForagedItemType0 / activeCount;
        double avg_countForagedItemType1 = (double)countForagedItemType1 / activeCount;

        double stddev_countForagedItemType0 = 0;
        double stddev_countForagedItemType1 = 0;
        
        for ( int i = 0 ; i != gNbOfRobots ; i++ )
        {
            ForagingRegionsController *ctl = dynamic_cast<ForagingRegionsController*>(gWorld->getRobot(i)->getController());
            
            if ( ctl->getWorldModel()->isAlive() == true )
            {
                stddev_countForagedItemType0 += pow( (double)ctl->nbForagedItemType0 - avg_countForagedItemType0, 2);
                stddev_countForagedItemType1 += pow( (double)ctl->nbForagedItemType1 - avg_countForagedItemType1, 2);
            }
        }
        
        stddev_countForagedItemType0 /= activeCount;
        stddev_countForagedItemType1 /= activeCount;
        
        sLitelog += ","
        + std::to_string(avg_countForagedItemType0)
        + ","
        + std::to_string(stddev_countForagedItemType0)
        + ","
        + std::to_string(avg_countForagedItemType1)
        + ","
        + std::to_string(stddev_countForagedItemType1);
    }
    
    //sLitelog += "\n";
    
    gLitelogManager->write(sLitelog);
    gLitelogFile << std::endl; // flush file output (+ "\n")
    gLitelogManager->flush();  // flush internal buffer to file
    
    // Logging, population-level: alive
    std::string sLog = std::string("") + std::to_string(gWorld->getIterations()) + ",pop,alive," + std::to_string(activeCount) + "\n";
    gLogManager->write(sLog);
    gLogManager->flush();

}
