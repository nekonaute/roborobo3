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
    
        double getFitness() override;

    protected:
    
        void initController() override;
        void stepController() override;
    
        void performSelection() override;
        void performVariation() override;
    
        void broadcastGenome() override;
    
        void resetFitness() override;
        void updateFitness() override;

        void logCurrentState() override;
};


#endif

