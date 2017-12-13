/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef TEMPLATEEEWORLDOBSERVER_H
#define TEMPLATEEEWORLDOBSERVER_H

#include "Observers/WorldObserver.h"

class World;

class TemplateEEWorldObserver : public WorldObserver
{
protected:
    virtual void updateEnvironment();
    virtual void updateMonitoring();
    virtual void monitorPopulation( bool localVerbose = true );
    
    int _generationCount;
    int _generationItCount;
    
public:
    TemplateEEWorldObserver(World *world);
    ~TemplateEEWorldObserver();
    
    void initPre() override;
    void initPost() override;

    void stepPre() override;
    void stepPost() override;

    virtual  int getGenerationItCount() { return _generationItCount; }

};

#endif
