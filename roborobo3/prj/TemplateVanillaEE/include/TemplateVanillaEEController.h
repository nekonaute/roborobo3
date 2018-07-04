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
    
        double getFitness() override;

    protected:
    
        void initController() override;
        void initGenome() override;
    
        void stepController() override;
    
        void performSelection() override;
        void performVariation() override;
    
        void broadcastGenome() override;
    
        bool sendGenome( TemplateEEController* __targetRobotController ) override;
        bool receiveGenome(std::vector<double> __genome, std::pair<int,int> __senderId, float __sigma=0, float __fitness=0) override;
    
        void resetFitness() override;
        void updateFitness() override;

        void logCurrentState() override;
};


#endif

