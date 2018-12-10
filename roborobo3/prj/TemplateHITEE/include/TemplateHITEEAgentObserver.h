/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef TEMPLATEHITEEAGENTOBSERVER_H
#define TEMPLATEHITEEAGENTOBSERVER_H

#include "TemplateEE/include/TemplateEEAgentObserver.h"

class RobotWorldModel;

class TemplateHITEEAgentObserver : public TemplateEEAgentObserver
{
	public:
		TemplateHITEEAgentObserver(RobotWorldModel *wm);
		~TemplateHITEEAgentObserver();
    
        void stepPre() override;
};

#endif

