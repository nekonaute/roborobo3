/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#include "TemplateEE/include/TemplateEESharedData.h"

double TemplateEESharedData::gSigmaMin = 0.0;
double TemplateEESharedData::gProbaMutation = 0.0;
double TemplateEESharedData::gUpdateSigmaStep = 0.0;
double TemplateEESharedData::gSigmaRef = 0.0; // reference value of sigma
double TemplateEESharedData::gSigmaMax = 0.0; // maximal value of sigma
int TemplateEESharedData::gEvaluationTime = 0; // how long a controller will be evaluated on a robot

bool TemplateEESharedData::gSynchronization = true;

bool TemplateEESharedData::gEnergyRequestOutput = 1;

double TemplateEESharedData::gMonitorPositions;

bool TemplateEESharedData::gPropertiesLoaded = false; // global variable local to file -- TODO: move specific properties loader in dedicated WorldObserver

int TemplateEESharedData::gNbHiddenLayers = 1;
int TemplateEESharedData::gNbNeuronsPerHiddenLayer = 5;
int TemplateEESharedData::gNeuronWeightRange = 800;

int TemplateEESharedData::gESNStepsBySimulationStep = 10;
int TemplateEESharedData::gESNReservoirSize = 100;
double TemplateEESharedData::gESNDensityOfConnections = 0.10;
double TemplateEESharedData::gESNAlpha = 0.88;
bool TemplateEESharedData::gESNAllowInputToOutputDirectConnections = false;
bool TemplateEESharedData::gESNFixedInputToReservoirConnections = true;
bool TemplateEESharedData::gESNAllowOutputSelfRecurrentConnections = true;
bool TemplateEESharedData::gESNAllowInputToReservoirConnections = true;
bool TemplateEESharedData::gESNAllowOutputToReservoirConnections = true;
bool TemplateEESharedData::gESNAddConstantInputBias = true;
bool TemplateEESharedData::gESNAddSinInputBias = false;
double TemplateEESharedData::gESNSinBiasPeriod = 0.01;
bool TemplateEESharedData::gESNUseSparseComputation = false;

bool TemplateEESharedData::gSnapshots = true; // take snapshots
int TemplateEESharedData::gSnapshotsFrequency = 50; // every N generations

int TemplateEESharedData::gControllerType = -1; // cf. header for description

bool TemplateEESharedData::gLimitGenomeTransmission = false; // default: do not limit.
int TemplateEESharedData::gMaxNbGenomeTransmission = 65535; // default: arbitrarily set to 65535.

int TemplateEESharedData::gSelectionMethod = 0; // default: random selection

int TemplateEESharedData::gNotListeningStateDelay = 0;    // -1: infinite ; 0: no delay ; >0: delay
int TemplateEESharedData::gListeningStateDelay = -1;      // -1: infinite ; 0: no delay ; >0: delay (ignored if gNotListeningStateDelay=-1)

bool TemplateEESharedData::gLogGenome = false;
bool TemplateEESharedData::gLogGenomeSnapshot = false;

double TemplateEESharedData::gIndividualMutationRate = 1.0;

int TemplateEESharedData::gMutationOperator = 1; // 0: uniform, 1: gaussian

double TemplateEESharedData::gSigma = 0.01; // 0.01 is just some random value.

std::ofstream gLitelogFile;
LogManager *gLitelogManager = NULL;
