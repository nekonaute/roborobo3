/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef TEMPLATEHITEEWORLDOBSERVER_H
#define TEMPLATEHITEEWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"

class World;

class TemplateHITEEWorldObserver : public TemplateEEWorldObserver
{
public:
    TemplateHITEEWorldObserver(World *world);
    ~TemplateHITEEWorldObserver();
    
    void initPre() override;
    void initPost() override;

    void stepPre() override;
    void stepPost() override;

protected:    
    void monitorPopulation( bool localVerbose = true ) override;
};

#endif
