/*
 *  PhysicalObjectFactory.h
 *  roborobo
 *
 *  Created by Nicolas on 25/4/2014.
 *
 */

#ifndef PHYSICALOBJECTFACTORY_H
#define PHYSICALOBJECTFACTORY_H

class PhysicalObject;

class PhysicalObjectFactory
{
	private :
        static int _nextId;
    
	public :
        static void makeObject( int type = 0 ); // default type is zero. Properties file value always override.
        static int getNextId();
        static int getNbOfTypes();
    

	
};

#endif
