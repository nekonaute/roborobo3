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
    
    void initPre() override;
    void initPost() override;

    void stepPre() override;
    void stepPost() override;

protected:    
    void monitorPopulation( bool localVerbose = true ) override;
};

#endif
