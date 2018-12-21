/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGREGIONSCONTROLLER_H
#define FORAGINGREGIONSCONTROLLER_H

#include "TemplateEE/include/TemplateEEController.h"
#include "ForagingRegions/include/ForagingRegionsPacket.h"

using namespace Neural;

class RobotWorldModel;

class ForagingRegionsController : public TemplateEEController
{
private:
    std::map< std::pair<int,int>, int > _regretValueList;
    
public:
    
    ForagingRegionsController(RobotWorldModel *wm);
    ~ForagingRegionsController();
    
    double getFitness() override;

    int nbForagedItemType0;
    int nbForagedItemType1;
    
    int* lastSeenObjectIdPerSensorList = NULL;
    //int lastSeenObjectIdOnFloorSensor = 0;
    
    int regret;
    
protected:
    
    void initController() override;
    void initEvolution() override;
    
    void stepController() override;
    void stepEvolution() override;
    
    void performSelection() override;
    void performVariation() override;
    
    void broadcastGenome() override;
    
    void resetFitness() override;
    void updateFitness() override;
    
    void logCurrentState() override;
    
    bool sendGenome( TemplateEEController* __targetRobotController ) override;
    bool receiveGenome( Packet* p ) override;
    
    void selectNaiveMO();
    
};


#endif

