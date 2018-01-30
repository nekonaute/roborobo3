/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGREGIONSCONTROLLER_H
#define FORAGINGREGIONSCONTROLLER_H

#include "TemplateEE/include/TemplateEEController.h"

using namespace Neural;

class RobotWorldModel;

class ForagingRegionsController : public TemplateEEController
{
    public:
    
        ForagingRegionsController(RobotWorldModel *wm);
        ~ForagingRegionsController();
    
        double getFitness();

        int nbForagedItemType0;
        int nbForagedItemType1;
    
        int* lastSeenObjectIdPerSensorList = NULL;
        //int lastSeenObjectIdOnFloorSensor = 0;
    
        int regret; 
    
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

