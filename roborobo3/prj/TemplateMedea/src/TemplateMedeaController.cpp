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
    // Note that in case of multiple encounters with the same robot (same id, same "birthdate"), genome is stored only once, and last known fitness value is stored (i.e. updated at each encounter).
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
