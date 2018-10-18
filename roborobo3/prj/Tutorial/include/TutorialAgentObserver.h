/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */
 
#ifndef TUTORIALAGENTOBSERVER_H
#define TUTORIALAGENTOBSERVER_H 

#include "Observers/AgentObserver.h"

class RobotWorldModel;

class TutorialAgentObserver : public AgentObserver
{
	public:
		TutorialAgentObserver( );
		TutorialAgentObserver( RobotWorldModel *__wm );
		~TutorialAgentObserver();
				
		void reset() override;
		void stepPre() override;
		
};


#endif

