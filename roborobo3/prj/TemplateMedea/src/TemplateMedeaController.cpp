/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateMedea/include/TemplateMedeaController.h"
#include "TemplateMedea/include/TemplateMedeaWorldObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include <math.h>
#include <string>

#include <neuralnetworks/MLP.h>
#include <neuralnetworks/Perceptron.h>
#include <neuralnetworks/Elman.h>

using namespace Neural;

TemplateMedeaController::TemplateMedeaController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // not used
}

TemplateMedeaController::~TemplateMedeaController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

void TemplateMedeaController::stepController()
{
    TemplateEEController::stepController();
}

void TemplateMedeaController::initController()
{
    TemplateEEController::initController();
}

void TemplateMedeaController::performSelection()
{
    TemplateEEController::performSelection();
}

void TemplateMedeaController::performVariation()
{
    TemplateEEController::performVariation();
}

void TemplateMedeaController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

double TemplateMedeaController::getFitness()
{
    // nothing to do
    // if relevant, use _wm->_fitnessValue.
    return -1;
}

void TemplateMedeaController::resetFitness()
{
    // nothing to do
}


void TemplateMedeaController::updateFitness()
{
    // nothing to do
}

void TemplateMedeaController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
