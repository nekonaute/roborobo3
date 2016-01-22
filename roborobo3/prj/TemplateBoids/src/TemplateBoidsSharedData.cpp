/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 * NNlib: Leo Cazenille <leo.cazenille@upmc.fr>
 */



#include "TemplateBoids/include/TemplateBoidsSharedData.h"


int TemplateBoidsSharedData::gEvaluationTime = 0; // how long a controller will be evaluated on a robot

double TemplateBoidsSharedData::gMonitorPositions;

bool TemplateBoidsSharedData::gPropertiesLoaded = false; // global variable local to file -- TODO: move specific properties loader in dedicated WorldObserver

bool TemplateBoidsSharedData::gSnapshots = true; // take snapshots
int TemplateBoidsSharedData::gSnapshotsFrequency = 50; // every N generations

