/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "ForagingRegions/include/ForagingRegionsController.h"
#include "ForagingRegions/include/ForagingRegionsSharedData.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

using namespace Neural;

ForagingRegionsController::ForagingRegionsController( RobotWorldModel *wm ) : TemplateEEController( wm )
{
    // superclass constructor called before this baseclass constructor.
    resetFitness(); // superconstructor calls parent method.
}

ForagingRegionsController::~ForagingRegionsController()
{
    // superclass destructor automatically called after this baseclass destructor.
}

void ForagingRegionsController::stepController()
{
    TemplateEEController::stepController();
}

void ForagingRegionsController::initController()
{
    TemplateEEController::initController();
}

void ForagingRegionsController::performSelection()
{
    TemplateEEController::performSelection();
}

void ForagingRegionsController::performVariation()
{
    TemplateEEController::performVariation();
}

void ForagingRegionsController::broadcastGenome()
{
    TemplateEEController::broadcastGenome();
}

double ForagingRegionsController::getFitness()
{
    return std::abs(_wm->_fitnessValue);
}

void ForagingRegionsController::resetFitness()
{
    TemplateEEController::resetFitness();
    
    if ( ForagingRegionsSharedData::foragingTask == 1 )
    {
        nbForagedItemType0 = 0;
        nbForagedItemType1 = 0;
    }
}

void ForagingRegionsController::updateFitness()
{
    // nothing to do -- updating is performed in ForagingRegionAgentObserver (automatic event when energy item are captured)
}

void ForagingRegionsController::logCurrentState()
{
    TemplateEEController::logCurrentState();
}
