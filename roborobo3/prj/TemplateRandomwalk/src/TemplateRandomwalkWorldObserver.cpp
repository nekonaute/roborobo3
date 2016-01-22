/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#include "TemplateRandomwalk/include/TemplateRandomwalkWorldObserver.h"

#include "World/World.h"

#include "TemplateRandomwalk/include/TemplateRandomwalkController.h"

TemplateRandomwalkWorldObserver::TemplateRandomwalkWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
}

TemplateRandomwalkWorldObserver::~TemplateRandomwalkWorldObserver()
{
	// nothing to do.
}

void TemplateRandomwalkWorldObserver::reset()
{
	// nothing to do.
}

void TemplateRandomwalkWorldObserver::step()
{
    
    // The following code shows an example where every 100 iterations, robots are re-located to their initial positions, and parameters are randomly changed.
    //
    // REMOVE OR COMMENT THE FOLLOWING TO AVOID RESETTING POSITIONS EVERY 100 ITERATIONS
    //
    
    if ( gWorld->getIterations() % 100 == 0 )
    {
        if ( gVerbose && gDisplayMode == 0 )
            std::cout << "Randomizing parameters\n";

        for ( int i = 0 ; i != gWorld->getNbOfRobots() ; i++ )
        {
            Robot *robot = (gWorld->getRobot(i));
            
            (*robot).setCoordReal( 100 + (i*50)%800 , 100 + ( i*50/800 * 50 )  );
            
            TemplateRandomwalkController *controller = ((TemplateRandomwalkController*)(gWorld->getRobot(i)->getController()));
            
            for ( size_t j = 0 ; j != (*controller)._params.size() ; j++ )
            {
                (*controller)._params[(int)j] = ranf()*2.0-1.0;
            }
        }
    }
    
}
