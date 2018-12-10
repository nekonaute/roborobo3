/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateHITEE/include/TemplateHITEEController.h"
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

void TemplateHITEEController::stepEvolution()
{
    TemplateEEController::stepEvolution();
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

bool TemplateHITEEController::sendGenome( TemplateEEController* __targetRobotController )
{
    return TemplateEEController::sendGenome(__targetRobotController);
}

bool TemplateHITEEController::receiveGenome( Packet* p )
{
    return TemplateEEController::receiveGenome(p);
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
