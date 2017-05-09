/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef TEMPLATEBOIDSWORLDOBSERVER_H
#define TEMPLATEBOIDSWORLDOBSERVER_H

#include "Observers/WorldObserver.h"

class TemplateBoidsWorldObserver : public WorldObserver
{
private:
    void updateEnvironment();
    void updateMonitoring();
    void monitorPopulation( bool localVerbose = true );
    
protected:
    int _generationCount;
    int _generationItCount;
    
public:
    TemplateBoidsWorldObserver(World *world);
    ~TemplateBoidsWorldObserver();
    
    void reset();
    void step();
    
    int getGenerationItCount() { return _generationItCount; }

};

#endif
