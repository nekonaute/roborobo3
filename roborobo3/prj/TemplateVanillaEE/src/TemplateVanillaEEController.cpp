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

void TemplateVanillaEEController::initGenome()
{
    TemplateEEController::initGenome();
}

void TemplateVanillaEEController::performSelection()
{
    TemplateEEController::performSelection();
}

void TemplateVanillaEEController::performVariation()
{
    TemplateEEController::performVariation();
}

void TemplateVanillaEEController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

bool TemplateVanillaEEController::sendGenome( TemplateEEController* __targetRobotController )
{
    return TemplateEEController::sendGenome(__targetRobotController);
}

bool TemplateVanillaEEController::receiveGenome(std::vector<double> __genome, std::pair<int,int> __senderId, float __sigma, float __fitness) // fitness is optional (default: 0)
{
    return TemplateEEController::receiveGenome(__genome, __senderId, __sigma, __fitness);
}

double TemplateVanillaEEController::getFitness()
{
    return TemplateEEController::getFitness();
}

void TemplateVanillaEEController::resetFitness()
{
    TemplateEEController::resetFitness();
}

void TemplateVanillaEEController::updateFitness()
{
    // nothing to do -- updating is performed in TemplateVanillaEEController::AgentObserver (automatic event when energy item are captured)
}

void TemplateVanillaEEController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
