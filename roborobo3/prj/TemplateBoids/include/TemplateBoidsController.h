/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 * NNlib: Leo Cazenille <leo.cazenille@upmc.fr>
 */



#ifndef TEMPLATEBOIDSCONTROLLER_H
#define TEMPLATEBOIDSCONTROLLER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/Graphics.h"
#include "Controllers/Controller.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateBoids/include/TemplateBoidsAgentObserver.h"
#include <neuralnetworks/NeuralNetwork.h>

#include <iomanip>

using namespace Neural;


class TemplateBoidsController : public Controller
{
private:
    int _iteration;
    void resetRobot();
    
public:
    
    TemplateBoidsController(RobotWorldModel *wm);
    ~TemplateBoidsController();
    
    void reset();
    void step();
};


#endif

