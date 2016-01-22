/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#include "TemplateWander/include/TemplateWanderWorldObserver.h"

#include "World/World.h"


TemplateWanderWorldObserver::TemplateWanderWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
}

TemplateWanderWorldObserver::~TemplateWanderWorldObserver()
{
	// nothing to do.
}

void TemplateWanderWorldObserver::reset()
{
	// nothing to do.
}

void TemplateWanderWorldObserver::step()
{
	// nothing to do.
}
