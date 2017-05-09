/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#ifndef TEMPLATEWANDERWORLDOBSERVER_H
#define TEMPLATEWANDERWORLDOBSERVER_H

#include "Observers/WorldObserver.h"

class World;

class TemplateWanderWorldObserver : public WorldObserver
{
	protected:
		
	public:
		TemplateWanderWorldObserver( World *__world );
		~TemplateWanderWorldObserver();
				
		void reset();
		void step();
		
};

#endif

