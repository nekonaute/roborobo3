/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "SandboxEE/include/SandboxEEController.h"
#include "SandboxEE/include/SandboxEEWorldObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include <math.h>
#include <string>

#include <neuralnetworks/MLP.h>
#include <neuralnetworks/Perceptron.h>
#include <neuralnetworks/Elman.h>

using namespace Neural;

SandboxEEController::SandboxEEController( RobotWorldModel *wm ) : TemplateMedeaController( wm )
{
    // superclass constructor called before this baseclass constructor.
}

SandboxEEController::~SandboxEEController()
{
    // superclass destructor automatically called after this baseclass destructor.
}


void SandboxEEController::stepController()
{
    TemplateMedeaController::stepController();
}

void SandboxEEController::initController()
{
    TemplateMedeaController::initController();
}


void SandboxEEController::performSelection()
{
    TemplateMedeaController::performSelection();
}

void SandboxEEController::performVariation()
{
    TemplateMedeaController::performVariation();
}


double SandboxEEController::getFitness()
{
    // nothing to do
    return -1;
}

void SandboxEEController::resetFitness()
{
    // nothing to do
}


void SandboxEEController::updateFitness()
{
    // nothing to do
}

