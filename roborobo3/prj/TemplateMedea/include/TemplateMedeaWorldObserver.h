/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */





#ifndef TEMPLATEMEDEAWORLDOBSERVER_H
#define TEMPLATEMEDEAWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/Observer.h"
#include "Observers/WorldObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateMedea/include/TemplateMedeaSharedData.h"
#include "TemplateEE/include/TemplateEEWorldObserver.h"

//class World;

class TemplateMedeaWorldObserver : public TemplateEEWorldObserver
{
public:
    TemplateMedeaWorldObserver(World *world);
    ~TemplateMedeaWorldObserver();
};

#endif
