/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef EECREDITASSIGNEMENTCONTROLLER_H
#define EECREDITASSIGNEMENTCONTROLLER_H

#include "TemplateEE/include/TemplateEEController.h"

using namespace Neural;

class RobotWorldModel;

class EEcreditassignementController : public TemplateEEController
{
    public:
    
        EEcreditassignementController(RobotWorldModel *wm);
        ~EEcreditassignementController();
    
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

