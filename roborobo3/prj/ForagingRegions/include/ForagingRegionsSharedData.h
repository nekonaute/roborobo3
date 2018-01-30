/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef FORAGINGREGIONSSHAREDDATA_H
#define FORAGINGREGIONSSHAREDDATA_H

#include "TemplateEE/include/TemplateEESharedData.h"

class ForagingRegionsSharedData : TemplateEESharedData {
    
    // cf. super class for many parameter values.
    // Add here parameters that are specific to current implementation.

public:
    static int nbObjectsType1;
    static int nbObjectsType2;
    static int foragingTask;
    static int fitnessFunction;
    static int regretValue;
}; 

#endif
