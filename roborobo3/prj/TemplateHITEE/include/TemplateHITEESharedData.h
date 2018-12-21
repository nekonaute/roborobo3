/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef TEMPLATEHITEESHAREDDATA_H
#define TEMPLATEHITEESHAREDDATA_H

#include "TemplateEE/include/TemplateEESharedData.h"

class TemplateHITEESharedData : TemplateEESharedData {
    
    // cf. super class for many parameter values.
    // Add here parameters that are specific to current implementation.
    
public:
    
    static double impactScale_HIT; // in [0.0,1.0]. If 0.0: exactly *one* gene sent. If 1.0: transmit full genome.
    
    static int maturationDelayDefaultValue; // suggest: superior or equal to memorySlidingWindowSize
    
    static int memorySlidingWindowSize; // 100...400
    static int coordSlidingWindowSize; // 3...10...400
    
    static int mutationOp;  // 0: uniform mutation ; 1: gaussian operator
    static double P_mutation; // ~ __1__ robot over __150__ mutates once every __400*10__ iterations for *each* robot.
    static bool resetOnMutation;
    static bool strictlySuperiorSelection; // accept if other.fitness > my.fitness or if other.fitness >= my.fitness? (default is false, i.e. ">=")
    
    static int fitnessFunction; // 0: collect ; 1: wander

}; 


#endif
