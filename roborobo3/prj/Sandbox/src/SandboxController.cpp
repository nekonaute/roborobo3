/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "Sandbox/include/SandboxController.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;

SandboxController::SandboxController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
}

SandboxController::~SandboxController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

void SandboxController::stepController()
{
    TemplateEEController::stepController();
}

void SandboxController::initController()
{
    TemplateEEController::initController();
}

void SandboxController::initEvolution()
{
    TemplateEEController::initEvolution();
}

void SandboxController::stepEvolution()
{
    TemplateEEController::stepEvolution();
}

void SandboxController::performSelection()
{
    TemplateEEController::performSelection();
}

void SandboxController::performVariation()
{
    TemplateEEController::performVariation();
}

void SandboxController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

bool SandboxController::sendGenome( TemplateEEController* __targetRobotController )
{
    return TemplateEEController::sendGenome(__targetRobotController);
}

bool SandboxController::receiveGenome( Packet* p )
{
    return TemplateEEController::receiveGenome(p);
}

double SandboxController::getFitness()
{
    return TemplateEEController::getFitness();
}

void SandboxController::resetFitness()
{
    TemplateEEController::resetFitness();
}

void SandboxController::updateFitness()
{
    // nothing to do -- updating is performed in SandboxController::AgentObserver (automatic event when energy item are captured)
}

void SandboxController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
