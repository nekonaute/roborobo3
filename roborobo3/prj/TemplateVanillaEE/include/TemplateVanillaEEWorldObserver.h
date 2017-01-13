/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */





#ifndef TEMPLATEVANILLAEEWORLDOBSERVER_H
#define TEMPLATEVANILLAEEWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/Observer.h"
#include "Observers/WorldObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateVanillaEE/include/TemplateVanillaEESharedData.h"
 
//class World;

class TemplateVanillaEEWorldObserver : public WorldObserver
{
protected:
    void updateEnvironment();
    void updateMonitoring();
    void monitorPopulation( bool localVerbose = true );
    
    int _generationCount;
    int _generationItCount;
    
public:
    TemplateVanillaEEWorldObserver(World *world);
    ~TemplateVanillaEEWorldObserver();
    
    void reset();
    void step();
    
    int getGenerationItCount() { return _generationItCount; }

};

#endif
