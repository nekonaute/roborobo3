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
#include "TemplateEE/include/TemplateEEWorldObserver.h"

//class World;

class TemplateVanillaEEWorldObserver : public TemplateEEWorldObserver
{
public:
    TemplateVanillaEEWorldObserver(World *world);
    ~TemplateVanillaEEWorldObserver();
};

#endif
