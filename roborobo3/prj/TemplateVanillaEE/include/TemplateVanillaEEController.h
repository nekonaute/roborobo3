/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef TEMPLATEVANILLAEECONTROLLER_H
#define TEMPLATEVANILLAEECONTROLLER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/Graphics.h"
#include "Controllers/Controller.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateVanillaEE/include/TemplateVanillaEEAgentObserver.h"
#include <neuralnetworks/NeuralNetwork.h>
#include "TemplateEE/include/TemplateEEController.h"

#include <iomanip>

using namespace Neural;


class TemplateVanillaEEController : public TemplateEEController
{
    public:
    
        TemplateVanillaEEController(RobotWorldModel *wm);
        ~TemplateVanillaEEController();
    
        double getFitness();

    protected:
    
        void initController();
        void stepController();
    
        void performSelection();
        void performVariation();
    
        void broadcastGenome();
    
        void resetFitness();
        void updateFitness();

        void logCurrentState();
};


#endif

