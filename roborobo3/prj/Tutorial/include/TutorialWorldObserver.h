/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#ifndef TUTORIALWORLDOBSERVER_H
#define TUTORIALWORLDOBSERVER_H

#include "Observers/WorldObserver.h"

class World;

class TutorialWorldObserver : public WorldObserver
{
	protected:
		
	public:
		TutorialWorldObserver( World *__world );
		~TutorialWorldObserver();
				
		void initPre();
        void initPost();

        void stepPre();
        void stepPost();
		
};

#endif

