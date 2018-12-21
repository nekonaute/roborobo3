/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef TEMPLATEHITEECONTROLLER_H
#define TEMPLATEHITEECONTROLLER_H

#include "TemplateEE/include/TemplateEEController.h"
#include "Utilities/Packet.h"
#include "Utilities/SlidingWindow.h"

using namespace Neural;

class RobotWorldModel;

class TemplateHITEEController : public TemplateEEController
{
    public:
    
        TemplateHITEEController(RobotWorldModel *wm);
        ~TemplateHITEEController();
    
        double getFitness() override;
        void updateFitness2(int value); // project-specific

    protected:
    
        int _maturationDelay;
        SlidingWindow* rewards;
        SlidingWindow* xCoords;
        SlidingWindow* yCoords;
        SlidingWindow* distances;

        int packetLength_HIT; // automatically set in constructor. in [ 1 , _currentGenome.size()-1 ]
    
        void initController() override;
        void initEvolution() override;
    
        void stepController() override;
        void stepEvolution() override;
    
        void performSelection() override;
        void performVariation() override;
    
        void broadcastGenome() override;
    
        bool sendGenome( TemplateEEController* __targetRobotController ) override;
        bool receiveGenome( Packet* p ) override;
    
        void resetFitness() override;
        void updateFitness() override;

        void logCurrentState() override;
 
};


#endif

