/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGREGIONSWORLDOBSERVER_H
#define FORAGINGREGIONSWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"

class World;

class ForagingRegionsWorldObserver : public TemplateEEWorldObserver
{
public:
    ForagingRegionsWorldObserver(World *world);
    ~ForagingRegionsWorldObserver();
    
    void initPre();
    void initPost();
    
    void stepPre();
    void stepPost();

protected:    
    virtual void monitorPopulation( bool localVerbose = true );
};

#endif
