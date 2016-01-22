/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 * NNlib: Leo Cazenille <leo.cazenille@upmc.fr>
 */



#include "TemplateMedea/include/TemplateMedeaSharedData.h"

double TemplateMedeaSharedData::gSigmaMin = 0.0;
double TemplateMedeaSharedData::gProbaMutation = 0.0;
double TemplateMedeaSharedData::gUpdateSigmaStep = 0.0;
double TemplateMedeaSharedData::gSigmaRef = 0.0; // reference value of sigma
double TemplateMedeaSharedData::gSigmaMax = 0.0; // maximal value of sigma
int TemplateMedeaSharedData::gEvaluationTime = 0; // how long a controller will be evaluated on a robot

bool TemplateMedeaSharedData::gSynchronization = true;

bool TemplateMedeaSharedData::gEnergyRequestOutput = 1;

double TemplateMedeaSharedData::gMonitorPositions;

bool TemplateMedeaSharedData::gPropertiesLoaded = false; // global variable local to file -- TODO: move specific properties loader in dedicated WorldObserver

int TemplateMedeaSharedData::gNbHiddenLayers = 1;
int TemplateMedeaSharedData::gNbNeuronsPerHiddenLayer = 5;
int TemplateMedeaSharedData::gNeuronWeightRange = 800;

bool TemplateMedeaSharedData::gSnapshots = true; // take snapshots
int TemplateMedeaSharedData::gSnapshotsFrequency = 50; // every N generations

int TemplateMedeaSharedData::gControllerType = -1; // cf. header for description

bool TemplateMedeaSharedData::gLimitGenomeTransmission = false; // default: do not limit.
int TemplateMedeaSharedData::gMaxNbGenomeTransmission = 65535; // default: arbitrarily set to 65535.

int TemplateMedeaSharedData::gSelectionMethod = 0; // default: random selection

int TemplateMedeaSharedData::gNotListeningStateDelay = 0;    // -1: infinite ; 0: no delay ; >0: delay
int TemplateMedeaSharedData::gListeningStateDelay = -1;      // -1: infinite ; 0: no delay ; >0: delay (ignored if gNotListeningStateDelay=-1)

bool TemplateMedeaSharedData::gLogGenome = false;

double TemplateMedeaSharedData::gIndividualMutationRate = 1.0;

int TemplateMedeaSharedData::gMutationOperator = 1; // 0: uniform, 1: gaussian

double TemplateMedeaSharedData::gSigma = 0.01; // 0.01 is just some random value.
