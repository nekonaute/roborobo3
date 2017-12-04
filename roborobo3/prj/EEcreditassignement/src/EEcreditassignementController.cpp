/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "EEcreditassignement/include/EEcreditassignementController.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;

EEcreditassignementController::EEcreditassignementController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
}

EEcreditassignementController::~EEcreditassignementController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

void EEcreditassignementController::stepController()
{
    TemplateEEController::stepController();
}

void EEcreditassignementController::initController()
{
    TemplateEEController::initController();
}

void EEcreditassignementController::performSelection()
{
    TemplateEEController::performSelection();
}

void EEcreditassignementController::performVariation()
{
    TemplateEEController::performVariation();
}

void EEcreditassignementController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

double EEcreditassignementController::getFitness()
{
    return TemplateEEController::getFitness();
}

void EEcreditassignementController::resetFitness()
{
    TemplateEEController::resetFitness();
}

void EEcreditassignementController::updateFitness()
{
    // nothing to do -- updating is performed in EEcreditassignementController::AgentObserver (automatic event when energy item are captured)
}

void EEcreditassignementController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
