/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateVanillaEE/include/TemplateVanillaEEController.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;

TemplateVanillaEEController::TemplateVanillaEEController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
}

TemplateVanillaEEController::~TemplateVanillaEEController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

void TemplateVanillaEEController::stepController()
{
    TemplateEEController::stepController();
}

void TemplateVanillaEEController::initController()
{
    TemplateEEController::initController();
}

void TemplateVanillaEEController::performSelection()
{
    //TemplateEEController::performSelection();
    
    std::pair<int,int> bestId;

    std::map<std::pair<int,int>, float >::iterator fitnessesIt = _fitnessValuesList.begin();

    float bestFitnessValue = (*fitnessesIt).second;
    bestId = (*fitnessesIt).first;

    ++fitnessesIt;

    int nbSimilar = 0;
    
    for ( int i = 1 ; fitnessesIt != _fitnessValuesList.end(); ++fitnessesIt, i++)
    {
        if ( (*fitnessesIt).second >= bestFitnessValue )
        {
            if ( (*fitnessesIt).second > bestFitnessValue )
            {
                bestFitnessValue = (*fitnessesIt).second;
                bestId = (*fitnessesIt).first;
                nbSimilar = 0;
            }
            else
            {
                nbSimilar++;
            }
        }
    }

    if ( nbSimilar > 0 ) // >1 genomes have the same fitness best value. Pick randomly among them
    {
        int count = 0;
        int randomPick = rand() % ( nbSimilar + 1 );
        
        if ( randomPick != 0 ) // not already stored (i.e. not the first one)
        {
            fitnessesIt = _fitnessValuesList.begin();
            for ( int i = 0 ; ; ++fitnessesIt, i++)
            {
                if ( (*fitnessesIt).second == bestFitnessValue )
                {
                    if ( count == randomPick )
                    {
                        bestId = (*fitnessesIt).first;
                        break;
                    }
                    count++;
                }
            }
        }
    }
    
    _birthdate = gWorld->getIterations();
    
    _currentGenome = _genomesList[bestId];
    _currentSigma = _sigmaList[bestId];
    
    setNewGenomeStatus(true);
    
    // Logging: track descendance
    std::string sLog = std::string("");
    sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",descendsFrom," + std::to_string((*_genomesList.begin()).first.first) + "::" + std::to_string((*_genomesList.begin()).first.second) + "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
}


void TemplateVanillaEEController::performVariation()
{
    TemplateEEController::performVariation();
}

void TemplateVanillaEEController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

double TemplateVanillaEEController::getFitness()
{
    return _wm->_fitnessValue;
}

void TemplateVanillaEEController::resetFitness()
{
    _wm->_fitnessValue = 0;
}


void TemplateVanillaEEController::updateFitness()
{
    // nothing to do -- updating is performed in AgentObserver (automatic event when energy item are captured)
}

void TemplateVanillaEEController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
