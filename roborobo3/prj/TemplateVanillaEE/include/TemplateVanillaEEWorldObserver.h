/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef TEMPLATEVANILLAEEWORLDOBSERVER_H
#define TEMPLATEVANILLAEEWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"

class World;

class TemplateVanillaEEWorldObserver : public TemplateEEWorldObserver
{
public:
    TemplateVanillaEEWorldObserver(World *world);
    ~TemplateVanillaEEWorldObserver();
    
    virtual void step();

protected:    
    virtual void monitorPopulation( bool localVerbose = true );
};

#endif
