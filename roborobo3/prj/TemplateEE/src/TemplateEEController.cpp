/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#include "TemplateEE/include/TemplateEEController.h"
#include "TemplateEE/include/TemplateEEWorldObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include <math.h>
#include <string>

#include <neuralnetworks/MLP.h>
#include <neuralnetworks/Perceptron.h>
#include <neuralnetworks/Elman.h>

using namespace Neural;

TemplateEEController::TemplateEEController( RobotWorldModel *wm )
{
    _wm = wm;
    
    nn = NULL;
    
    // evolutionary engine
    
    _minValue = -1.0;
    _maxValue = 1.0;
    
    _currentSigma = TemplateEESharedData::gSigmaRef;
    
    // behaviour
    
    _iteration = 0;
    
    _birthdate = 0;
    
    _isListening = true;
    _notListeningDelay = TemplateEESharedData::gNotListeningStateDelay;
    _listeningDelay = TemplateEESharedData::gListeningStateDelay;
    
    _nbGenomeTransmission = 0;
    
    if ( gEnergyLevel )
        _wm->setEnergyLevel(gEnergyInit);
    
    if ( gLandmarks.size() > 0 )
        _wm->updateLandmarkSensor(); // wrt closest landmark
    
    reset();
    
    _wm->setAlive(true);
    _wm->setRobotLED_colorValues(255, 0, 0);
    
}

TemplateEEController::~TemplateEEController()
{
    _parameters.clear();
    delete nn;
    nn = NULL;
}

void TemplateEEController::step() // handles control decision and evolution (but: actual movement is done in roborobo's main loop)
{
    _iteration++;
    
    // * step evolution
    
    stepEvolution();
    
    // * step controller
    
    if ( _wm->isAlive() )
    {
        stepController();
        updateFitness();
    }
    else
    {
        _wm->_desiredTranslationalValue = 0.0;
        _wm->_desiredRotationalVelocity = 0.0;
    }
    
    // * updating listening state
    
    if ( _wm->isAlive() == false )
    {
        assert ( _notListeningDelay >= -1 ); // -1 means infinity
        
        if ( _notListeningDelay > 0 )
        {
            _notListeningDelay--;
            
            if ( _notListeningDelay == 0 )
            {
                
                _listeningDelay = TemplateEESharedData::gListeningStateDelay;
                
                if ( _listeningDelay > 0 || _listeningDelay == -1 )
                {
                    _isListening = true;
                    
                    _wm->setRobotLED_colorValues(0, 255, 0); // is listening
                    
                    std::string sLog = std::string("");
                    sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",status,listening\n";
                    gLogManager->write(sLog);
                    gLogManager->flush();
                }
                else
                {
                    std::string sLog = std::string("");
                    sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",status,inactive\n"; // never listen again.
                    gLogManager->write(sLog);
                    gLogManager->flush();
                }
            }
        }
        else
            if ( _notListeningDelay != -1 && _listeningDelay > 0 )
            {
                assert ( _isListening == true );
                
                _listeningDelay--;
                
                if ( _listeningDelay == 0 )
                {
                    _isListening = false;
                    // Logging: robot is dead
                    std::string sLog = std::string("");
                    sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",status,inactive\n";
                    gLogManager->write(sLog);
                    gLogManager->flush();

                    _notListeningDelay = -1; // agent will not be able to be active anymore
                    _wm->setRobotLED_colorValues(0, 0, 255); // is not listening
                    
                    reset(); // destroy then create a new NN
                    
                    _wm->setAlive(false);
                }
            }
    }
}


// ################ ######################## ################
// ################ ######################## ################
// ################ BEHAVIOUR METHOD(S)      ################
// ################ ######################## ################
// ################ ######################## ################


std::vector<double> TemplateEEController::getInputs(){
    
    
    // WHAT FOLLOWS IS AN EXAMPLE OF LOADING A NN-CONTROLER WITH A FULL-FLEDGE SENSORY INPUT INFORMATION
    // Rewrite to match your own extended input scheme, if needed.
    // Note that you may tune it on/off using gExtendedSensoryInputs defined in the properties file.
    // When modifying this code, dont forget to update the initialization in the reset() method
    // Example:
    //      - you may want to distinguish between robot's groups (if more than 1)
    //      - you may want to restrict the number of objects that can be identified (if not all possible objects are in use)
    //      - you may want to compress inputs (e.g. binary encoding instead of one-input-per-object-type.
    //      - (...)
    //
    // In the following, sensory inputs for each sensor are ordered (and filled in) as follow:
    // - N range sensors
    //      - distance to obstacle (max if nothing)
    //      IF extendedInputs is True:
    //          - [0...N_physicalobjecttypes] Is it an object of type i? (0: no, 1: yes) -- type: 0 (round), 1 (energy item), 2 (gate), 3 (switch), ...? (cf. PhysicalObjectFactory)
    //          - is it a robot? (1 or 0)
    //          - is it from the same group? (1 or 0)
    //          - relative orientation wrt. current robot (relative orientation or 0 if not a robot)
    //          - is it a wall? (ie. a non-distinguishible something) (1 or 0)
    // - floor sensor, red value
    // - floor sensor, green value
    // - floor sensor, blue value
    // - relative direction to the closest landmark
    // - distance to the closest landmark
    // - normalized energy level (if any)
    //
    // => number of sensory inputs: N * rangeSensors + 6, with rangeSensors varying from 1 to many, if extended sensory inputs are on.
    

    
    std::vector<double> inputs;
    
    // distance sensors
    for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
    {
        inputs.push_back( _wm->getDistanceValueFromCameraSensor(i) / _wm->getCameraSensorMaximumDistanceValue(i) );
        
        if ( gExtendedSensoryInputs ) // EXTENDED SENSORY INPUTS: code provided as example, should be rewritten to suit your need.
        {
            int objectId = _wm->getObjectIdFromCameraSensor(i);
            
            // input: physical object? which type?
            if ( PhysicalObject::isInstanceOf(objectId) )
            {
                int nbOfTypes = PhysicalObjectFactory::getNbOfTypes();
                for ( int i = 0 ; i != nbOfTypes ; i++ )
                {
                    if ( i == gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() )
                        inputs.push_back( 1 ); // match
                    else
                        inputs.push_back( 0 );
                }
            }
            else
            {
                // not a physical object. But: should still fill in the inputs (with zeroes)
                int nbOfTypes = PhysicalObjectFactory::getNbOfTypes();
                for ( int i = 0 ; i != nbOfTypes ; i++ )
                {
                    inputs.push_back( 0 );
                }
            }
            
            // input: another agent? If yes: same group?
            if ( Agent::isInstanceOf(objectId) )
            {
                // this is an agent
                inputs.push_back( 1 );
                
                // same group?
                if ( gWorld->getRobot(objectId-gRobotIndexStartOffset)->getWorldModel()->getGroupId() == _wm->getGroupId() )
                {
                    inputs.push_back( 1 ); // match: same group
                }
                else
                {
                    inputs.push_back( 0 ); // not the same group
                }
                
                // relative orientation? (ie. angle difference wrt. current agent)
                double srcOrientation = _wm->_agentAbsoluteOrientation;
                double tgtOrientation = gWorld->getRobot(objectId-gRobotIndexStartOffset)->getWorldModel()->_agentAbsoluteOrientation;
                double delta_orientation = - ( srcOrientation - tgtOrientation );
                if ( delta_orientation >= 180.0 )
                    delta_orientation = - ( 360.0 - delta_orientation );
                else
                    if ( delta_orientation <= -180.0 )
                        delta_orientation = - ( - 360.0 - delta_orientation );
                inputs.push_back( delta_orientation/180.0 );
            }
            else
            {
                inputs.push_back( 0 ); // not an agent...
                inputs.push_back( 0 ); // ...therefore no match wrt. group.
                inputs.push_back( 0 ); // ...and no orientation.
            }
            
            // input: wall or empty?
            if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ) // not empty, but cannot be identified: this is a wall.
                inputs.push_back( 1 );
            else
                inputs.push_back( 0 ); // nothing. (objectId=-1)
        }
    }
    
    
    // floor sensor
    inputs.push_back( (double)_wm->getGroundSensor_redValue()/255.0 );
    inputs.push_back( (double)_wm->getGroundSensor_greenValue()/255.0 );
    inputs.push_back( (double)_wm->getGroundSensor_blueValue()/255.0 );
    
    // landmark (closest landmark)
    if ( gLandmarks.size() > 0 )
    {
        _wm->updateLandmarkSensor(); // update with closest landmark
        inputs.push_back( _wm->getLandmarkDirectionAngleValue() );
        inputs.push_back( _wm->getLandmarkDistanceValue() );
    }
    
    
    // energy level
    if ( gEnergyLevel )
    {
        inputs.push_back( _wm->getEnergyLevel() / gEnergyMax );
    }
    
    return inputs;
}

void TemplateEEController::stepController()
{

    // ---- compute and read out ----
    
    nn->setWeigths(_parameters); // set-up NN
    
    std::vector<double> inputs = getInputs(); // Build list of inputs (check properties file for extended/non-extended input values
    
    nn->setInputs(inputs);
    
    nn->step();
    
    std::vector<double> outputs = nn->readOut();
    
    // std::cout << "[DEBUG] Neural Network :" << nn->toString() << " of size=" << nn->getRequiredNumberOfWeights() << std::endl;
    
    _wm->_desiredTranslationalValue = outputs[0];
    _wm->_desiredRotationalVelocity = outputs[1];
    
    if ( TemplateEESharedData::gEnergyRequestOutput )
    {
        _wm->setEnergyRequestValue(outputs[2]);
    }
    
    // normalize to motor interval values
    _wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue * gMaxTranslationalSpeed;
    _wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity * gMaxRotationalSpeed;
    
}


void TemplateEEController::createNN()
{
    setIOcontrollerSize(); // compute #inputs and #outputs
    
    if ( nn != NULL ) // useless: delete will anyway check if nn is NULL or not.
        delete nn;
    
    switch ( TemplateEESharedData::gControllerType )
    {
        case 0:
        {
            // MLP
            nn = new MLP(_parameters, _nbInputs, _nbOutputs, *(_nbNeuronsPerHiddenLayer));
            break;
        }
        case 1:
        {
            // PERCEPTRON
            nn = new Perceptron(_parameters, _nbInputs, _nbOutputs);
            break;
        }
        case 2:
        {
            // ELMAN
            nn = new Elman(_parameters, _nbInputs, _nbOutputs, *(_nbNeuronsPerHiddenLayer));
            break;
        }
        default: // default: no controller
            std::cerr << "[ERROR] gController type unknown (value: " << TemplateEESharedData::gControllerType << ").\n";
            exit(-1);
    };
}


unsigned int TemplateEEController::computeRequiredNumberOfWeights()
{
    unsigned int res = nn->getRequiredNumberOfWeights();
    return res;
}

// ################ ######################## ################
// ################ ######################## ################
// ################ EVOLUTION ENGINE METHODS ################
// ################ ######################## ################
// ################ ######################## ################

void TemplateEEController::stepEvolution()
{
    
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % TemplateEESharedData::gEvaluationTime == 0 )
                    //dynamic_cast<TemplateEEWorldObserver*>(gWorld->getWorldObserver())->getGenerationItCount() == 0 )  //todelete
    {
        // * lifetime ended: replace genome (if possible)
        loadNewGenome();
        _nbGenomeTransmission = 0;
    }
    else
    {
        // * broadcasting genome : robot broadcasts its genome to all neighbors (contact-based wrt proximity sensors)
        // note: no broadcast if last iteration before replacement -- this is enforced as roborobo update method is random-asynchroneous. This means that robots broadcasting may transmit genomes to robot already at the next generation depending on the update ordering (should be avoided).
        if ( _wm->isAlive() == true && gRadioNetwork )  	// only if agent is active (ie. not just revived) and deltaE>0.
        {
            broadcastGenome();
        }

        _dSumTravelled = _dSumTravelled + getEuclidianDistance( _wm->getXReal(), _wm->getYReal(), _Xinit, _Yinit ); //remark: incl. squareroot.
    }
    
    // log the genome (only at the second iteration during snapshot time)
    if ( TemplateEESharedData::gLogGenome && gWorld->getIterations() % ( TemplateEESharedData::gEvaluationTime * TemplateEESharedData::gSnapshotsFrequency ) == 1 )
    {
        // Logging: full genome
        std::string sLog = std::string("");
        sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",genome,";
        if ( _wm->isAlive() )
        {
            for(unsigned int i=0; i<_currentGenome.size(); i++)
            {
                sLog += std::to_string(_currentGenome[i]);
                if ( i < _currentGenome.size()-1 )
                    sLog += ",";
            }
        }
        else
            sLog += "n/a"; // do not write genome
        
        sLog += "\n";
        gLogManager->write(sLog);
        gLogManager->flush();
    }
    
    
    if ( getNewGenomeStatus() ) // check for new NN parameters
    {
        _parameters.clear();
        _parameters = _currentGenome;
        setNewGenomeStatus(false);
    }
}

void TemplateEEController::performVariation()
{
    if ( TemplateEESharedData::gIndividualMutationRate > rand()/RAND_MAX ) // global mutation rate (whether this genome will get any mutation or not) - default: always
    {
        switch ( TemplateEESharedData::gMutationOperator )
        {
            case 0:
                mutateUniform();
                break;
            case 1:
                mutateSigmaValue(); // self-contained sigma. mutated before use (assume: performVariation is called after selection of new current genome)
                mutateGaussian(_currentSigma); // original MEDEA [ppsn2010], as used before year 2015
                break;
            case 2:
                mutateGaussian(TemplateEESharedData::gSigma); // fixed mutation rate
                break;
            default:
                std::cerr << "[ERROR] unknown variation method (gMutationOperator = " << TemplateEESharedData::gMutationOperator << ")\n";
                exit(-1);
        }
    }
}

void TemplateEEController::selectRandomGenome() // if called, assume genomeList.size()>0
{
    int randomIndex = rand()%_genomesList.size();

    std::map<std::pair<int,int>, std::vector<double> >::iterator it = _genomesList.begin();
    while (randomIndex !=0 )
    {
        it ++;
        randomIndex --;
    }
    
    _currentGenome = (*it).second;
    _currentSigma = _sigmaList[(*it).first];
    _birthdate = gWorld->getIterations();
    
    setNewGenomeStatus(true); 
    
    // Logging: track descendance
    std::string sLog = std::string("");
    sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",descendsFrom," + std::to_string((*_genomesList.begin()).first.first) + "::" + std::to_string((*_genomesList.begin()).first.second) + "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
}

void TemplateEEController::selectFirstGenome()  // if called, assume genomeList.size()>0
{
    _currentGenome = (*_genomesList.begin()).second;
    _currentSigma = _sigmaList[(*_genomesList.begin()).first];
    _birthdate = gWorld->getIterations();
    
    setNewGenomeStatus(true);
    
    // Logging: track descendance
    std::string sLog = std::string("");
    sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",descendsFrom," + std::to_string((*_genomesList.begin()).first.first) + "::" + std::to_string((*_genomesList.begin()).first.second) + "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
}

/* manage storage of a genome received from a neighbour
 *
 * Note that in case of multiple encounters with the same robot (same id, same "birthdate"), genome is stored only once, and last known fitness value is stored (i.e. updated at each encounter).
 */
bool TemplateEEController::storeGenome(std::vector<double> genome, std::pair<int,int> senderId, float sigma, float fitness) // fitness is optional (default: 0)
{
    if ( !_isListening )
    {
        return false; // current agent is not listening: do nothing.
    }
    else
    {
        std::map<std::pair<int,int>, std::vector<double> >::const_iterator it = _genomesList.find(senderId);
    
        /*
        _genomesList[senderId] = genome;
        _sigmaList[senderId] = sigma;
        _fitnessValuesList[senderId] = fitness;
        */
        
        if ( it != _genomesList.end() ) // this exact agent's genome is already stored. Exact means: same robot, same generation. Then: update fitness value (the rest in unchanged)
        {
            _fitnessValuesList[senderId] = fitness; // update with most recent fitness (IMPLEMENTATION CHOICE) [!n]
            return false;
        }
        else
        {
            _genomesList[senderId] = genome;
            _sigmaList[senderId] = sigma;
            _fitnessValuesList[senderId] = fitness;
            return true;
        }
    }
}


void TemplateEEController::mutateGaussian(float sigma) // mutate within bounds.
{
    _currentSigma = sigma;
    
    for (unsigned int i = 0 ; i != _currentGenome.size() ; i++ )
    {
        double value = _currentGenome[i] + getGaussianRand(0,_currentSigma);
        // bouncing upper/lower bounds
        if ( value < _minValue )
        {
            double range = _maxValue - _minValue;
            double overflow = - ( (double)value - _minValue );
            overflow = overflow - 2*range * (int)( overflow / (2*range) );
            if ( overflow < range )
                value = _minValue + overflow;
            else // overflow btw range and range*2
                value = _minValue + range - (overflow-range);
        }
        else if ( value > _maxValue )
        {
            double range = _maxValue - _minValue;
            double overflow = (double)value - _maxValue;
            overflow = overflow - 2*range * (int)( overflow / (2*range) );
            if ( overflow < range )
                value = _maxValue - overflow;
            else // overflow btw range and range*2
                value = _maxValue - range + (overflow-range);
        }
        
        _currentGenome[i] = value;
    }
    
}


void TemplateEEController::mutateUniform() // mutate within bounds.
{
    for (unsigned int i = 0 ; i != _currentGenome.size() ; i++ )
    {
        float randomValue = float(rand()%100) / 100.0; // in [0,1[
        double range = _maxValue - _minValue;
        double value = randomValue * range + _minValue;
        
        _currentGenome[i] = value;
    }
}


void TemplateEEController::setIOcontrollerSize()
{
    // wrt inputs
    
    _nbInputs = 0;
    
    if ( gExtendedSensoryInputs ) // EXTENDED SENSORY INPUTS: code provided as example, can be rewritten to suit your need.
    {
        _nbInputs = ( PhysicalObjectFactory::getNbOfTypes()+3+1 ) * _wm->_cameraSensorsNb; // nbOfTypes + ( isItAnAgent? + isItSameGroupId? + agentAngleDifference?) + isItAWall?
    }
    
    _nbInputs += _wm->_cameraSensorsNb + 3; // proximity sensors + ground sensor (3 values)
    if ( gEnergyLevel )
        _nbInputs += 1; // incl. energy level
    if ( gLandmarks.size() > 0 )
        _nbInputs += 2; // incl. landmark (angle,dist)
    
    // wrt outputs
    
    _nbOutputs = 2;
    
    if ( TemplateEESharedData::gEnergyRequestOutput )
        _nbOutputs += 1; // incl. energy request
}

void TemplateEEController::initController()
{
    _nbHiddenLayers = TemplateEESharedData::gNbHiddenLayers;
    _nbNeuronsPerHiddenLayer = new std::vector<unsigned int>(_nbHiddenLayers);
    for(unsigned int i = 0; i < _nbHiddenLayers; i++)
        (*_nbNeuronsPerHiddenLayer)[i] = TemplateEESharedData::gNbNeuronsPerHiddenLayer;
    
    createNN();

    unsigned int const nbGene = computeRequiredNumberOfWeights();
    
    if ( gVerbose )
        std::cout << std::flush ;
    
    _currentGenome.clear();
    
    // Intialize genome
    
    for ( unsigned int i = 0 ; i != nbGene ; i++ )
    {
        _currentGenome.push_back((double)(rand()%TemplateEESharedData::gNeuronWeightRange)/(TemplateEESharedData::gNeuronWeightRange/2)-1.0); // weights: random init between -1 and +1
    }
    
    setNewGenomeStatus(true);
    
    clearReservoir(); // will contain the genomes received from other robots
}


void TemplateEEController::clearReservoir()
{
    //std::cout << "[DEBUG] genomesList for agent #" << _wm->getId() << "::" << getBirthdate() << ", at time " << gWorld->getIterations() << "\n";
    //for ( std::map<std::pair<int,int>, std::vector<double> >::iterator it = _genomesList.begin() ; it != _genomesList.end() ; it++ )
    //    std::cout << "[DEBUG] " << (*it).first.first << "::" << (*it).first.second << "\n";
    
    _genomesList.clear(); // empty the list of received genomes
    _sigmaList.clear();
    _fitnessValuesList.clear();
}

void TemplateEEController::reset()
{
    initController();
    resetFitness();
}


void TemplateEEController::mutateSigmaValue()
{
    float dice = float(rand()%100) / 100.0;
    
    if ( dice <= TemplateEESharedData::gProbaMutation )
    {
        dice = float(rand() %100) / 100.0;
        if ( dice < 0.5 )
        {
            _currentSigma = _currentSigma * ( 1 + TemplateEESharedData::gUpdateSigmaStep ); // increase sigma
            
            if (_currentSigma > TemplateEESharedData::gSigmaMax)
            {
                _currentSigma = TemplateEESharedData::gSigmaMax;
            }
        }
        else
        {
            _currentSigma = _currentSigma * ( 1 - TemplateEESharedData::gUpdateSigmaStep ); // decrease sigma
            
            if ( _currentSigma < TemplateEESharedData::gSigmaMin )
            {
                _currentSigma = TemplateEESharedData::gSigmaMin;
            }
        }
    }
}


void TemplateEEController::broadcastGenome()
{
    // remarque \todo: limiting genome transmission is sensitive to sensor order. (but: assume ok)
    
    for( int i = 0 ; i < _wm->_cameraSensorsNb && ( TemplateEESharedData::gLimitGenomeTransmission == false || ( TemplateEESharedData::gLimitGenomeTransmission == true && _nbGenomeTransmission < TemplateEESharedData::gMaxNbGenomeTransmission ) ); i++)
    {
        int targetIndex = _wm->getObjectIdFromCameraSensor(i);
        
        if ( targetIndex >= gRobotIndexStartOffset && targetIndex < gRobotIndexStartOffset+gNumberOfRobots )   // sensor ray bumped into a robot : communication is possible
        {
            targetIndex = targetIndex - gRobotIndexStartOffset; // convert image registering index into robot id.
            
            TemplateEEController* targetRobotController = dynamic_cast<TemplateEEController*>(gWorld->getRobot(targetIndex)->getController());
            
            if ( ! targetRobotController )
            {
                std::cerr << "Error from robot " << _wm->getId() << " : the observer of robot " << targetIndex << " is not compatible." << std::endl;
                exit(-1);
            }
            
            if ( targetRobotController->isListening() )
            {
                bool success = targetRobotController->storeGenome(_currentGenome, std::make_pair(_wm->getId(), _birthdate), _currentSigma, _wm->_fitnessValue); // other agent stores my genome. Contaminant stragegy. Note that medea does not use fitnessValue (default value: 0)
                
                if ( success == true )
                    _nbGenomeTransmission++; // count unique transmissions (ie. nb of different genomes stored).
            }
        }
    }
}

void TemplateEEController::performSelection()
{
    switch ( TemplateEESharedData::gSelectionMethod )
    {
        case 0:
            selectRandomGenome();
            break;
        case 1:
            selectFirstGenome();
            break;
        default:
            std::cerr << "[ERROR] unknown selection method (gSelectionMethod = " << TemplateEESharedData::gSelectionMethod << ")\n";
            exit(-1);
    }
}


void TemplateEEController::loadNewGenome()
{
    if ( _wm->isAlive() || gEnergyRefill )  // ( gEnergyRefill == true ) enables revive
    {
        if ( _wm->isAlive() )
            logCurrentState(); // TODO: 2016-05-30n - check modif pour amaury
        
        // note: at this point, agent got energy, whether because it was revived or because of remaining energy.
        
        if (_genomesList.size() > 0)
        {
            // case: 1+ genome(s) imported, random pick.
            
            performSelection();
            performVariation();
            clearReservoir();
            
            //logCurrentState();
            
            _wm->setAlive(true);
            
            std::string sLog = std::string("");
            sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",status,active\n";
            gLogManager->write(sLog);
            gLogManager->flush();
            
            if ( _wm->getEnergyLevel() == 0 )
                _wm->setEnergyLevel(gEnergyInit);
            
            _Xinit = _wm->getXReal();
            _Yinit = _wm->getYReal();
            _dSumTravelled = 0;
            
            _wm->setRobotLED_colorValues(255, 0, 0);
            
            // log the genome (or at least the existence of a genome)
            if ( _wm->isAlive() )
            {
                // Logging: full genome
                std::string sLog = std::string("");
                sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",genome,";
                
                /*
                 for(unsigned int i=0; i<_genome.size(); i++)
                 {
                 sLog += std::to_string(_genome[i]) + ",";
                 //gLogFile << std::fixed << std::showpoint << _wm->_genome[i] << " ";
                 }
                 */
                sLog += "(...)"; // do not write genome
                
                sLog += "\n";
                gLogManager->write(sLog);
                gLogManager->flush();
            }
        }
        else
        {
            // case: no imported genome and the robot is/was active - robot is set to inactive (which means: robot is put off-line (if gDeathState is true), then wait for new genome (if gListenState is true))
            
            if ( _wm->isAlive() == true )
            {
                
                // Logging: "no genome"
                std::string sLog = std::string("");
                sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",genome,n/a.\n";
                gLogManager->write(sLog);
                gLogManager->flush();
                
                reset(); // destroy then create a new NN
                
                _wm->setAlive(false); // inactive robot *must* import a genome from others (ie. no restart).
                
                if ( TemplateEESharedData::gNotListeningStateDelay != 0 ) // ie. -1 (infinite,dead) or >0 (temporary,mute)
                {
                    _isListening = false;

                    _notListeningDelay = TemplateEESharedData::gNotListeningStateDelay;
                    _listeningDelay = TemplateEESharedData::gListeningStateDelay;
                    _wm->setRobotLED_colorValues(0, 0, 255); // is not listening
                    
                    std::string sLog = std::string("");
                    sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",status,inactive\n";
                    gLogManager->write(sLog);
                    gLogManager->flush();
                    
                }
                else
                {
                    _listeningDelay = TemplateEESharedData::gListeningStateDelay;

                    if ( _listeningDelay > 0 || _listeningDelay == -1 )
                    {
                        _isListening = true;
                        
                        _wm->setRobotLED_colorValues(0, 255, 0); // is listening
                        
                        std::string sLog = std::string("");
                        sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",status,listening\n";
                        gLogManager->write(sLog);
                        gLogManager->flush();
                    }
                    else
                    {
                        std::string sLog = std::string("");
                        sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",status,inactive\n";
                        gLogManager->write(sLog);
                        gLogManager->flush();
                    }
                }
            }
        }
    }
}

void TemplateEEController::logCurrentState()
{
    // Logging
    std::string sLog = "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) +
    ",age," + std::to_string(gWorld->getIterations()-_birthdate) +
    ",energy," +  std::to_string(_wm->getEnergyLevel()) +
    ",genomesListSize," + std::to_string(_genomesList.size()) +
    ",sigma," + std::to_string(_currentSigma) +
    ",x_init," + std::to_string(_wm->getXReal()) +
    ",y_init," + std::to_string(_wm->getYReal()) +
    ",x_current," + std::to_string(_Xinit) +
    ",y_current," + std::to_string(_Yinit) +
    ",dist," + std::to_string( getEuclidianDistance( _Xinit, _Yinit, _wm->getXReal(), _wm->getYReal() ) ) +
    ",sumOfDist," + std::to_string( _dSumTravelled ) +
    ",groupId," + std::to_string(_wm->getGroupId()) +
    ",fitnessValue," + std::to_string(_wm->_fitnessValue) + 
    "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
}

double TemplateEEController::getFitness()
{
    // nothing to do
    return -1;
}

/*
 * note: resetFitness is first called by the Controller's constructor.
 */
void TemplateEEController::resetFitness()
{
    // nothing to do
}


void TemplateEEController::updateFitness()
{
    // nothing to do
}

