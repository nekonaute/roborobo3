/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef TEMPLATEVANILLAEEAGENTOBSERVER_H
#define TEMPLATEVANILLAEEAGENTOBSERVER_H

#include "TemplateEE/include/TemplateEEAgentObserver.h"

class RobotWorldModel;

class TemplateVanillaEEAgentObserver : public TemplateEEAgentObserver
{
	public:
		TemplateVanillaEEAgentObserver(RobotWorldModel *wm);
		~TemplateVanillaEEAgentObserver();
    
        virtual void step();
};

#endif

