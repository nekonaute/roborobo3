/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef TEMPLATEVANILLAEECONTROLLER_H
#define TEMPLATEVANILLAEECONTROLLER_H

#include "TemplateEE/include/TemplateEEController.h"

using namespace Neural;

class RobotWorldModel;

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

