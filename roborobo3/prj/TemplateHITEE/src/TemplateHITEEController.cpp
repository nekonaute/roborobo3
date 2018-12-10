/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateHITEE/include/TemplateHITEEController.h"
#include "TemplateHITEE/include/TemplateHITEESharedData.h"

#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;

TemplateHITEEController::TemplateHITEEController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
}

TemplateHITEEController::~TemplateHITEEController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

void TemplateHITEEController::stepController()
{
    TemplateEEController::stepController();
}

void TemplateHITEEController::initController()
{
    TemplateEEController::initController();
}

void TemplateHITEEController::initEvolution()
{
    TemplateEEController::initEvolution();
}


void TemplateHITEEController::performSelection()
{
    TemplateEEController::performSelection();
}

void TemplateHITEEController::performVariation()
{
    TemplateEEController::performVariation();
}

void TemplateHITEEController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

double TemplateHITEEController::getFitness()
{
    return TemplateEEController::getFitness();
}

void TemplateHITEEController::resetFitness()
{
    TemplateEEController::resetFitness();
}

void TemplateHITEEController::updateFitness()
{
    // nothing to do -- updating is performed in TemplateHITEEController::AgentObserver (automatic event when energy item are captured)
}

void TemplateHITEEController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}

// -------
// -------
// -------

void TemplateHITEEController::stepEvolution()
{
    // Do NOT call TemplateEEController::stepEvolution()
    
    clearReservoir(); // [!n] check when should be done (possibly inbetween world iteration? if done here: not synced. though async is ok.

    if ( _wm->isAlive() == true ) // in HIT-EE, always true.
    {
        // send genome to neighbours
        broadcastGenome();

        // with a (very) small probability, apply uniform mutation to one gene
        int randomIndex = randint() % _currentGenome.size();
        double P_uniformMutation = 0.0025; // ~one gene mutation every 400 iterations. // TODO: value should be fixed by user in properties file
        if ( random01() < P_uniformMutation )
        {
            double range = _maxValue - _minValue;
            double value = random01() * range + _minValue;
            _currentGenome[randomIndex] = value;
            setNewGenomeStatus(true);
        }

    }
    
    // logging
    _dSumTravelled = _dSumTravelled + getEuclideanDistance( _wm->getXReal(), _wm->getYReal(), _Xinit, _Yinit ); //remark: incl. squareroot.
    
    if ( getNewGenomeStatus() ) // check for new NN parameters
    {
        mapGenotypeToPhenotype();
        setNewGenomeStatus(false);
    }
    
    resetFitness(); // Very brute: we consider fitness to be 0 or 1. Benefit can only be immediate. Smoothed over long period, that might makes sense, though.
}

// -----
// -----
// -----


bool TemplateHITEEController::sendGenome( TemplateEEController* __targetRobotController )
{
    // future implementations: conditional send.

    // other agent stores my genome. Contaminant stragegy.
    // Note that medea does not use fitnessValue (default value: 0)
    // if you need to send other information, derive your own Packet class and sendGenome/receiveGenome methods in your project space.

    Packet* p = new Packet();
    p->senderId = std::make_pair(_wm->getId(), _birthdate);
    p->fitness = getFitness();
    p->genome = _currentGenome;
    p->sigma = _currentSigma;
    
    bool retValue = ((TemplateHITEEController*)__targetRobotController)->receiveGenome(p);
    
    delete p;
    
    return retValue;
}

// -----
// -----
// -----

// Remark: receiveGenome is called only if robot's listening mode is on (ie. _isListening == true).
bool TemplateHITEEController::receiveGenome( Packet* p )
{
    // future implementations: conditional receive&integrate.
    
    std::map<std::pair<int,int>, std::vector<double> >::const_iterator it = _genomesList.find(p->senderId);
    if ( it == _genomesList.end() ) // check never met since last listening reset (same robot, same generation)
    {
        if ( p->fitness >= _wm->_fitnessValue )
        {
            int randomIndex = randint() % _currentGenome.size();
            _currentGenome[randomIndex] = p->genome[randomIndex];
            // p->sigma;
            // p->fitness;
            setNewGenomeStatus(true);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false; // already met. Ignored.
    }
}
