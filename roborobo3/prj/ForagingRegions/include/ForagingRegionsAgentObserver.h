/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef FORAGINGREGIONSAGENTOBSERVER_H
#define FORAGINGREGIONSAGENTOBSERVER_H

#include "TemplateEE/include/TemplateEEAgentObserver.h"

class RobotWorldModel;

class ForagingRegionsAgentObserver : public TemplateEEAgentObserver
{
	public:
		ForagingRegionsAgentObserver(RobotWorldModel *wm);
		~ForagingRegionsAgentObserver();
    
        virtual void step();
};

#endif

