//
//  SensoryInformation.h
//  roborobo3
//
//  Created by Nicolas Bredeche on 03/11/16.
//  Copyright © 2016 Nicolas Bredeche. All rights reserved.
//

#ifndef SENSORYINFORMATION_H
#define SENSORYINFORMATION_H

class SensoryInformation
{

private:
    
    //double absoluteOrientation; // wrt North (aka: up-screen)

    bool isWall
    
    bool isRobot;
    bool isRobot;
    int robotId;
    double robotOrientation; // wrt North
    double delta_orientation; // wrt current robot
    
    bool isObject
    int objectType; // cf. PhysicalObjectFactory -- 0 (round), 1 (energy item), 2 (gate), 3 (switch), ...?
    
    //double groundSensorValues[3]; // RGB
    
    //double landmarkDirectionAngleValue;
    //double landmarkDistanceValue;

    //double energyLevel;

public:

    SensoryInformation();
    ~SensoryInformation();
};


#endif /* SENSORYINFORMATION_H */
