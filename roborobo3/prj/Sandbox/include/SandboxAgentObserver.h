/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef SANDBOXAGENTOBSERVER_H
#define SANDBOXAGENTOBSERVER_H

#include "TemplateEE/include/TemplateEEAgentObserver.h"

class RobotWorldModel;

class SandboxAgentObserver : public TemplateEEAgentObserver
{
	public:
		SandboxAgentObserver(RobotWorldModel *wm);
		~SandboxAgentObserver();
    
        void stepPre() override;
};

#endif

