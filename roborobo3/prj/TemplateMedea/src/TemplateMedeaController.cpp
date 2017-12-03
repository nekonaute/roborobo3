/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateMedea/include/TemplateMedeaController.h"

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
    // mEDEA uses no fitness. Set to zero by default.
    return 0;
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
