/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef EECREDITASSIGNEMENTAGENTOBSERVER_H
#define EECREDITASSIGNEMENTAGENTOBSERVER_H

#include "TemplateEE/include/TemplateEEAgentObserver.h"

class RobotWorldModel;

class EEcreditassignementAgentObserver : public TemplateEEAgentObserver
{
	public:
		EEcreditassignementAgentObserver(RobotWorldModel *wm);
		~EEcreditassignementAgentObserver();
    
        virtual void step();
};

#endif

