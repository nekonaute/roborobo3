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
#include "Utilities/Misc.h"

ForagingRegionsWorldObserver::ForagingRegionsWorldObserver( World* world ) : TemplateEEWorldObserver( world )
{
    // superclass constructor called before

    gProperties.checkAndGetPropertyValue("gNbObjectsType1",&ForagingRegionsSharedData::nbObjectsType1,true);
    gProperties.checkAndGetPropertyValue("gNbObjectsType2",&ForagingRegionsSharedData::nbObjectsType2,true);
    gProperties.checkAndGetPropertyValue("gForagingTask",&ForagingRegionsSharedData::foragingTask,true);
    
    gProperties.checkAndGetPropertyValue("fitnessFunction",&ForagingRegionsSharedData::fitnessFunction,true);
    gProperties.checkAndGetPropertyValue("regretValue",&ForagingRegionsSharedData::regretValue,true);
    
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# [0]:generation,[1]:iteration,[2]:populationSize,[3]:minFitness,[4]:maxFitness,[5]:avgFitnessNormalized,[6]:sumOfFitnesses,[7]:foragingBalance,[8]:avg_countForagedItemType0,[9]:stddev_countForagedItemType0, [10]:avg_countForagedItemType1,[11]:stddev_countForagedItemType1,[12]:globalWelfare,[13]minGenomeReservoirSize,[14]maxGenomeReservoirSize,[15]avgGenomeReservoirSize,[16]avgForagingBalancePerRobot,[17]avgForagingBalancePerRobot,[18]activeCountWithForaging.\n");
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
    int activeCountWithForaging = 0; // count active robots that forage at least one item.
    
    double sumOfFitnesses = 0;
    double minFitness = DBL_MAX;
    double maxFitness = -DBL_MAX;
    
    int countForagedItemType0 = 0;
    int countForagedItemType1 = 0;
    
    int minGenomeReservoirSize = -1;
    int maxGenomeReservoirSize = -1;
    double avgGenomeReservoirSize = -1;
    
    double avgForagingBalancePerRobot = 0;
    double stddev_activeCountWithForaging = 0;
    
    for ( int i = 0 ; i != gNbOfRobots ; i++ )
    {
        ForagingRegionsController *ctl = dynamic_cast<ForagingRegionsController*>(gWorld->getRobot(i)->getController());
        
        if ( ctl->getWorldModel()->isAlive() == true )
        {
            activeCount++;

            // fitnesses
            
            sumOfFitnesses += ctl->getFitness() ;
            if ( ctl->getFitness() < minFitness )
                minFitness = ctl->getFitness();
            if ( ctl->getFitness() > maxFitness )
                maxFitness = ctl->getFitness();
            
            // foraging scores
            
            countForagedItemType0 += ctl->nbForagedItemType0;
            countForagedItemType1 += ctl->nbForagedItemType1;
            
            // balancing between resources, agent-level
            
            if ( ctl->nbForagedItemType0 + ctl->nbForagedItemType1 > 0 )
            {
                avgForagingBalancePerRobot += getBalance(ctl->nbForagedItemType0,ctl->nbForagedItemType1);
                activeCountWithForaging++;
            }

            // genome reservoir sizes
            
            int genomeReservoirSize = ctl->getGenomeReservoirSize();
            
            avgGenomeReservoirSize += (double)genomeReservoirSize;
            
            if ( minGenomeReservoirSize == -1 )
            {
                minGenomeReservoirSize = genomeReservoirSize;
                maxGenomeReservoirSize = genomeReservoirSize;
            }
            else
            {
                if ( minGenomeReservoirSize > genomeReservoirSize )
                    minGenomeReservoirSize = genomeReservoirSize;
                else
                    if ( maxGenomeReservoirSize < genomeReservoirSize )
                        maxGenomeReservoirSize = genomeReservoirSize;
            }
        }
    }
    
    avgGenomeReservoirSize = avgGenomeReservoirSize / activeCount;
    
    avgForagingBalancePerRobot = avgForagingBalancePerRobot / activeCountWithForaging; // robot-level, consider active robots with foraging activity.
    
    double foragingBalance = getBalance( countForagedItemType0 , countForagedItemType1 ); // pop-level
    
    double avgFitnessNormalized;
    
    if ( activeCount == 0 ) // arbitrary convention: in case of extinction, min/max/avg fitness values are -1
    {
        minFitness = -1;
        maxFitness = -1;
        avgFitnessNormalized = -1;
    }
    else
        avgFitnessNormalized = sumOfFitnesses/activeCount;
    
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
            
            if ( ctl->nbForagedItemType0 + ctl->nbForagedItemType1 > 0 )
                stddev_activeCountWithForaging += pow( getBalance(ctl->nbForagedItemType0,ctl->nbForagedItemType1) - avgForagingBalancePerRobot, 2);
        }
    }
    
    stddev_countForagedItemType0 /= activeCount;
    stddev_countForagedItemType1 /= activeCount;
    
    stddev_activeCountWithForaging /= avgForagingBalancePerRobot;
    
    sLitelog += ","
    + std::to_string(avg_countForagedItemType0)
    + ","
    + std::to_string(stddev_countForagedItemType0)
    + ","
    + std::to_string(avg_countForagedItemType1)
    + ","
    + std::to_string(stddev_countForagedItemType1)
    + ","
    + std::to_string(countForagedItemType0+countForagedItemType1); // ie. global welfare, or: population-level foraging score

    sLitelog += ","
    + std::to_string(minGenomeReservoirSize)
    + ","
    + std::to_string(maxGenomeReservoirSize)
    + ","
    + std::to_string(avgGenomeReservoirSize);
    
    sLitelog += ","
    + std::to_string(avgForagingBalancePerRobot)
    + ","
    + std::to_string(stddev_activeCountWithForaging)
    + ","
    + std::to_string(activeCountWithForaging);
    
    gLitelogManager->write(sLitelog);
    gLitelogFile << std::endl; // flush file output (+ "\n")
    gLitelogManager->flush();  // flush internal buffer to file
    
    // Logging, population-level: alive
    std::string sLog = std::string("") + std::to_string(gWorld->getIterations()) + ",pop,alive," + std::to_string(activeCount) + "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
    
    if ( gVerbose && localVerbose )
    {
        std::cout << "[ gen:" << (gWorld->getIterations()/TemplateEESharedData::gEvaluationTime) << "\tit:" << gWorld->getIterations() << "\tpop:" << activeCount << "\tminFitness:" << minFitness << "\tmaxFitness:" << maxFitness << "\tavgFitnessNormalized:" << avgFitnessNormalized << "\tglobalWelfare:" << (countForagedItemType0+countForagedItemType1) << " ]\n";
    }

}


