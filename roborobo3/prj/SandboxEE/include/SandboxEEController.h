/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef SANDBOXEECONTROLLER_H
#define SANDBOXEECONTROLLER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/Graphics.h"
#include "Controllers/Controller.h"
#include "WorldModels/RobotWorldModel.h"
#include "SandboxEE/include/SandboxEEAgentObserver.h"
#include <neuralnetworks/NeuralNetwork.h>

#include "TemplateMedea/include/TemplateMedeaController.h"

#include <iomanip>

using namespace Neural;


class SandboxEEController : public TemplateMedeaController
{
    public:
        SandboxEEController(RobotWorldModel *wm);
        ~SandboxEEController();
    
    protected:
        void initController();
        void stepController();
    
        void performSelection();
        void performVariation();
    
        double getFitness();
        void resetFitness();
        void updateFitness();

};


#endif

