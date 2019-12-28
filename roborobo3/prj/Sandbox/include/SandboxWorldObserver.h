/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef SANDBOXWORLDOBSERVER_H
#define SANDBOXWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"

class World;

class SandboxWorldObserver : public TemplateEEWorldObserver
{
public:
    SandboxWorldObserver(World *world);
    ~SandboxWorldObserver();
    
    void initPre() override;
    void initPost() override;

    void stepPre() override;
    void stepPost() override;

protected:    
    void monitorPopulation( bool localVerbose = true ) override;
};

#endif
