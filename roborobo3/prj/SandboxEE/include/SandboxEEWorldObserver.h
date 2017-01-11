/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */





#ifndef SANDBOXEEWORLDOBSERVER_H
#define SANDBOXEEWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/Observer.h"
#include "Observers/WorldObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "SandboxEE/include/SandboxEESharedData.h"
 
//class World;

class SandboxEEWorldObserver : public WorldObserver
{
protected:
    void updateEnvironment();
    void updateMonitoring();
    void monitorPopulation( bool localVerbose = true );
    
    int _generationCount;
    int _generationItCount;
    
public:
    SandboxEEWorldObserver(World *world);
    ~SandboxEEWorldObserver();
    
    void reset();
    void step();
    
    int getGenerationItCount() { return _generationItCount; }

};

#endif
