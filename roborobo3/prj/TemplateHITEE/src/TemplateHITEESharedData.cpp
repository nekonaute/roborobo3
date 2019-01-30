/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#include "TemplateHITEE/include/TemplateHITEESharedData.h"

// cf. super class for many parameter values initialization.
// Add here initialization for parameters that are specific to current implementation.
//
// Quick help:
//  if parameter is already defined in TemplateEEShareData, then use TemplateEEShareData::parametername
//  to define a new parameter, do it in TemplateHITEESharedData.h, initialize it in TemplateHITEESharedData.cpp, then use TemplateHITEESharedData::parametername
//

double TemplateHITEESharedData::transferVolume = 0.0;
double TemplateHITEESharedData::mutationVolume = 0.0;
int TemplateHITEESharedData::maturationDelayDefaultValue = 400;
int TemplateHITEESharedData::memorySlidingWindowSize = 400;
int TemplateHITEESharedData::coordSlidingWindowSize = 400;
int TemplateHITEESharedData::mutationOp = 0;
double TemplateHITEESharedData::P_mutation = 1.0/150/400/10;
bool TemplateHITEESharedData::resetOnMutation = false;
bool TemplateHITEESharedData::strictlySuperiorSelection = false;
int TemplateHITEESharedData::fitnessFunction = 0;
