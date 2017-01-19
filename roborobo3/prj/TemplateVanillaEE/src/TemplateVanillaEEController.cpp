/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateVanillaEE/include/TemplateVanillaEEController.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEWorldObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include <math.h>
#include <string>

#include <neuralnetworks/MLP.h>
#include <neuralnetworks/Perceptron.h>
#include <neuralnetworks/Elman.h>

using namespace Neural;

TemplateVanillaEEController::TemplateVanillaEEController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
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
    
    float bestFitnessValue = 0;
    std::pair<int,int> bestId;

    int i = 0;
    for (std::map<std::pair<int,int>, float >::iterator fitnessesIt = _fitnessValuesList.begin(); fitnessesIt != _fitnessValuesList.end(); ++fitnessesIt, i++)
    {
        if ( i == 0 )
        {
            bestFitnessValue = (*fitnessesIt).second;
            bestId = (*fitnessesIt).first;
        }
        else
            if ( (*fitnessesIt).second > bestFitnessValue )
            {
                bestFitnessValue = (*fitnessesIt).second;
                bestId = (*fitnessesIt).first;
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
