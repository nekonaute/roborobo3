/*
 *  WorldModel.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef WORLDMODEL_H
#define WORLDMODEL_H 

class WorldModel
{
private:
    bool _isAlive;  // robot behavior is executed *only* if robot is alived. (default is: alive)
    
public:
	//Initializes the variables
	WorldModel();
	~WorldModel();
    
    void setAlive( bool value ) { _isAlive = value; }
    bool isAlive() { return _isAlive; }
};


#endif


