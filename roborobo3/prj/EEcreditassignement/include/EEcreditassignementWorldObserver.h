/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef EECREDITASSIGNEMENTWORLDOBSERVER_H
#define EECREDITASSIGNEMENTWORLDOBSERVER_H

#include "TemplateEE/include/TemplateEEWorldObserver.h"

class World;

class EEcreditassignementWorldObserver : public TemplateEEWorldObserver
{
public:
    EEcreditassignementWorldObserver(World *world);
    ~EEcreditassignementWorldObserver();
    
    void initPre();
    void initPost();
    
    void stepPre();
    void stepPost();

protected:    
    virtual void monitorPopulation( bool localVerbose = true );
};

#endif
