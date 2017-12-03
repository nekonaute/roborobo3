/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateEE/include/TemplateEEWorldObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "World/World.h"
#include "TemplateEE/include/TemplateEEController.h"
#include "RoboroboMain/roborobo.h"
#include "TemplateEE/include/TemplateEESharedData.h"
#include "Utilities/Graphics.h"

TemplateEEWorldObserver::TemplateEEWorldObserver( World* world ) : WorldObserver( world )
{
    _world = world;
    
    // ==== create specific "lite" logger file
    
    std::string litelogFullFilename = gLogDirectoryname + "/lite_" + gLogFilename;
    gLitelogFile.open(litelogFullFilename.c_str());
        
    if(!gLitelogFile) {
        std::cout << "[CRITICAL] Cannot open \"lite\" log file " << litelogFullFilename << "." << std::endl << std::endl;
        exit(-1);
    }
        
    gLitelogManager = new LogManager();
    gLitelogManager->setLogFile(gLitelogFile);
    gLitelogManager->write("# lite logger\n");
    gLitelogManager->write("# generation,iteration,populationSize,minFitness,maxFitness,avgFitness.\n");
    gLitelogManager->flush();

    // ==== loading project-specific properties

    gProperties.checkAndGetPropertyValue("gSigmaRef",&TemplateEESharedData::gSigmaRef,true);
    gProperties.checkAndGetPropertyValue("gSigmaMin",&TemplateEESharedData::gSigmaMin,true);
    gProperties.checkAndGetPropertyValue("gSigmaMax",&TemplateEESharedData::gSigmaMax,true);
    
    gProperties.checkAndGetPropertyValue("gProbaMutation",&TemplateEESharedData::gProbaMutation,true);
    gProperties.checkAndGetPropertyValue("gUpdateSigmaStep",&TemplateEESharedData::gUpdateSigmaStep,true);
    gProperties.checkAndGetPropertyValue("gEvaluationTime",&TemplateEESharedData::gEvaluationTime,true);
    gProperties.checkAndGetPropertyValue("gSynchronization",&TemplateEESharedData::gSynchronization,true);
    
    gProperties.checkAndGetPropertyValue("gEnergyRequestOutput",&TemplateEESharedData::gEnergyRequestOutput,false);
    
    gProperties.checkAndGetPropertyValue("gMonitorPositions",&TemplateEESharedData::gMonitorPositions,true);
    
    gProperties.checkAndGetPropertyValue("gNbHiddenLayers",&TemplateEESharedData::gNbHiddenLayers,true);
    gProperties.checkAndGetPropertyValue("gNbNeuronsPerHiddenLayer",&TemplateEESharedData::gNbNeuronsPerHiddenLayer,true);
    gProperties.checkAndGetPropertyValue("gNeuronWeightRange",&TemplateEESharedData::gNeuronWeightRange,true);
    
    gProperties.checkAndGetPropertyValue("gSnapshots",&TemplateEESharedData::gSnapshots,false);
    gProperties.checkAndGetPropertyValue("gSnapshotsFrequency",&TemplateEESharedData::gSnapshotsFrequency,false);
    
    gProperties.checkAndGetPropertyValue("gControllerType",&TemplateEESharedData::gControllerType,true);
    
    gProperties.checkAndGetPropertyValue("gMaxNbGenomeTransmission",&TemplateEESharedData::gMaxNbGenomeTransmission,true);
    gProperties.checkAndGetPropertyValue("gLimitGenomeTransmission",&TemplateEESharedData::gLimitGenomeTransmission,true);
    gProperties.checkAndGetPropertyValue("gSelectionMethod",&TemplateEESharedData::gSelectionMethod,true);
    
    gProperties.checkAndGetPropertyValue("gNotListeningStateDelay",&TemplateEESharedData::gNotListeningStateDelay,true);
    gProperties.checkAndGetPropertyValue("gListeningStateDelay",&TemplateEESharedData::gListeningStateDelay,true);
    
    gProperties.checkAndGetPropertyValue("gLogGenome",&TemplateEESharedData::gLogGenome,false);
    gProperties.checkAndGetPropertyValue("gLogGenomeSnapshot",&TemplateEESharedData::gLogGenomeSnapshot,false);
    
    gProperties.checkAndGetPropertyValue("gIndividualMutationRate",&TemplateEESharedData::gIndividualMutationRate,false);

    gProperties.checkAndGetPropertyValue("gMutationOperator",&TemplateEESharedData::gMutationOperator,false);
    
    gProperties.checkAndGetPropertyValue("gSigma",&TemplateEESharedData::gSigma,false);
    
    // ====
    
    if ( !gRadioNetwork)
    {
        std::cout << "Error : gRadioNetwork must be true." << std::endl;
        exit(-1);
    }
    
    // * iteration and generation counters
    
    _generationItCount = -1;
    _generationCount = -1;
}

TemplateEEWorldObserver::~TemplateEEWorldObserver()
{
    gLitelogFile.close();
}

void TemplateEEWorldObserver::reset()
{
}

void TemplateEEWorldObserver::stepPre()
{
    _generationItCount++;
    
    if( _generationItCount == TemplateEESharedData::gEvaluationTime+1 ) // switch to next generation.
    {
        // update iterations and generations counters
        _generationItCount = 0;
        _generationCount++;
    }
    
    updateMonitoring();
    
    updateEnvironment();
    
}

void TemplateEEWorldObserver::stepPost()
{
    // nothing to do.
}

void TemplateEEWorldObserver::updateEnvironment()
{
    // example: moving landmarks
    /*
    if ( gWorld->getIterations() % 2000 == 0 )
        for ( int i = 0 ; i != gNbOfLandmarks ; i++ )
        {
            Point2d* position = new Point2d( 200+randint()%(gAreaWidth-400) , 200+randint()%(gAreaHeight-400) );
            gLandmarks[i]->setPosition(*position);
        }
    */
}

void TemplateEEWorldObserver::updateMonitoring()
{
    // * Log at end of each generation

    //if( gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == 1 || gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == TemplateEESharedData::gEvaluationTime-1 ) // beginning(+1) *and* end of generation. ("==1" is required to monitor the outcome of the first iteration)
    // log at end of generation.
    if( gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == TemplateEESharedData::gEvaluationTime-1 )
    {
        monitorPopulation();
    }
    
    // * Every N generations, take a video (duration: one generation time)
    
    if ( TemplateEESharedData::gSnapshots )
    {
        if ( ( gWorld->getIterations() ) % ( TemplateEESharedData::gEvaluationTime * TemplateEESharedData::gSnapshotsFrequency ) == 0 )
        {
            if ( gVerbose )
                std::cout << "[START] Video recording: generation #" << (gWorld->getIterations() / TemplateEESharedData::gEvaluationTime ) << ".\n";
            gTrajectoryMonitorMode = 0;
            initTrajectoriesMonitor();
        }
        else
            if ( ( gWorld->getIterations() ) % ( TemplateEESharedData::gEvaluationTime * TemplateEESharedData::gSnapshotsFrequency ) == TemplateEESharedData::gEvaluationTime - 1 )
            {
                if ( gVerbose )
                    std::cout << "[STOP]  Video recording: generation #" << (gWorld->getIterations() / TemplateEESharedData::gEvaluationTime ) << ".\n";
                saveTrajectoryImage();
            }
    }    
}

void TemplateEEWorldObserver::monitorPopulation( bool localVerbose )
{
    // * monitoring: count number of active agents.
    
    int activeCount = 0;
    double sumOfFitnesses = 0;
    double minFitness = DBL_MAX;
    double maxFitness = -DBL_MAX;
    
    for ( int i = 0 ; i != gNbOfRobots ; i++ )
    {
        TemplateEEController *ctl = dynamic_cast<TemplateEEController*>(gWorld->getRobot(i)->getController());
        
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
        std::cout << "[ gen:" << (gWorld->getIterations()/TemplateEESharedData::gEvaluationTime) << "\tit:" << gWorld->getIterations() << "\tpop:" << activeCount << "\tminFitness:" << minFitness << "\tmaxFitness:" << maxFitness << "\tavgFitness:" << sumOfFitnesses/activeCount << " ]\n";
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
        + std::to_string(sumOfFitnesses/activeCount)
        + "\n";
    gLitelogManager->write(sLitelog);
    gLitelogManager->flush();
    
    // Logging, population-level: alive
    std::string sLog = std::string("") + std::to_string(gWorld->getIterations()) + ",pop,alive," + std::to_string(activeCount) + "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
}
