/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef TEMPLATEMEDEACONTROLLER_H
#define TEMPLATEMEDEACONTROLLER_H

#include "TemplateEE/include/TemplateEEController.h"

using namespace Neural;

class RobotWorldModel;

class TemplateMedeaController : public TemplateEEController
{
    public:
    
        TemplateMedeaController(RobotWorldModel *wm);
        ~TemplateMedeaController();
    
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

