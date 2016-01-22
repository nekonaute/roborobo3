/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 * NNlib: Leo Cazenille <leo.cazenille@upmc.fr>
 */

#include "TemplateMedea/include/TemplateMedeaController.h"
#include "TemplateMedea/include/TemplateMedeaWorldObserver.h"

#include "World/World.h"
#include "Utilities/Misc.h"
#include <math.h>
#include <string>

#include <neuralnetworks/MLP.h>
#include <neuralnetworks/Perceptron.h>
#include <neuralnetworks/Elman.h>

using namespace Neural;

TemplateMedeaController::TemplateMedeaController( RobotWorldModel *wm )
{
    _wm = wm;
    
    nn = NULL;
    
    // evolutionary engine
    
    _minValue = -1.0;
    _maxValue = 1.0;
    
    _currentSigma = TemplateMedeaSharedData::gSigmaRef;
    
    resetRobot();
    
    // behaviour
    
    _iteration = 0;
    
    _birthdate = 0;
    
    _isListening = true;
    _notListeningDelay = TemplateMedeaSharedData::gNotListeningStateDelay;
    _listeningDelay = TemplateMedeaSharedData::gListeningStateDelay;
    
    _nbGenomeTransmission = 0;
    
    if ( gEnergyLevel )
        _wm->setEnergyLevel(gEnergyInit);
    
    _wm->updateLandmarkSensor(); // wrt closest landmark
    
    _wm->setAlive(true);
    _wm->setRobotLED_colorValues(255, 0, 0);
    
}

TemplateMedeaController::~TemplateMedeaController()
{
    _parameters.clear();
    delete nn;
    nn = NULL;
}

void TemplateMedeaController::reset()
{
    _parameters.clear();
    _parameters = _genome;
}


void TemplateMedeaController::step()
{
    _iteration++;
    
    // * step evolution
    
    stepEvolution();
    
    // * step controller
    
    if ( _wm->isAlive() )
    {
        stepBehaviour();
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
                
                _listeningDelay = TemplateMedeaSharedData::gListeningStateDelay;
                
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
                    
                    resetRobot(); // destroy then create a new NN
                    
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


void TemplateMedeaController::stepBehaviour()
{
    _wm->updateLandmarkSensor(); // update with closest landmark
    
    // ---- Build inputs ----
    
    std::vector<double>* inputs = new std::vector<double>(_nbInputs);
    int inputToUse = 0;
    
    // distance sensors
    for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
    {
        (*inputs)[inputToUse] = _wm->getDistanceValueFromCameraSensor(i) / _wm->getCameraSensorMaximumDistanceValue(i);
        inputToUse++;
        
        if ( gExtendedSensoryInputs ) // EXTENDED SENSORY INPUTS: code provided as example, should be rewritten to suit your need.
        {
            // WHAT FOLLOWS IS AN EXAMPLE OF EXTENDED SENSORY INPUTS
            // Rewrite to match your own extended input scheme, if needed.
            // You may tune it on/off using gExtendedSensoryInputs defined in the properties file.
            // When modifying this code, dont forget to update the initialization in the resetRobot() method
            // Example:
            //      - you may want to distinguish between robot's groups (if more than 1)
            //      - you may want to restrict the number of objects that can be identified (if not all possible objects are in use)
            //      - you may want to compress inputs (e.g. binary encoding instead of one-input-per-object-type.
            //      - (...)
            //
            // In the following
            //      - inputs[N_physicalobjecttypes]: 0 or 1 is active, other are set to zero.
            //      - inputs[2]: (a) is it a robot? (b) is it from the same group? (c) what is its relative orientation wrt. current robot
            //      - inputs[1]: is it a wall (=1), or nothing (=0)
            //      Comment: from 0 (nothing) to 2 (robot, same group) active inputs.
            
            int objectId = _wm->getObjectIdFromCameraSensor(i);
            
            // input: physical object? which type?
            if ( PhysicalObject::isInstanceOf(objectId) )
            {
                int nbOfTypes = PhysicalObjectFactory::getNbOfTypes();
                for ( int i = 0 ; i != nbOfTypes ; i++ )
                {
                    if ( i == gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() )
                        (*inputs)[inputToUse] = 1; // match
                    else
                        (*inputs)[inputToUse] = 0;
                    inputToUse++;
                }
            }
            else
            {
                // not a physical object. But: should still fill in the inputs (with zeroes)
                int nbOfTypes = PhysicalObjectFactory::getNbOfTypes();
                for ( int i = 0 ; i != nbOfTypes ; i++ )
                {
                    (*inputs)[inputToUse] = 0;
                    inputToUse++;
                }
            }
            
            // input: another agent? If yes: same group?
            if ( Agent::isInstanceOf(objectId) )
            {
                // this is an agent
                (*inputs)[inputToUse] = 1;
                inputToUse++;
                
                // same group?
                if ( gWorld->getRobot(objectId-gRobotIndexStartOffset)->getWorldModel()->getGroupId() == _wm->getGroupId() )
                {
                    (*inputs)[inputToUse] = 1; // match: same group
                }
                else
                {
                    (*inputs)[inputToUse] = 0; // not the same group
                }
                inputToUse++;
                
                // relative orientation? (ie. angle difference wrt. current agent)
                double srcOrientation = _wm->_agentAbsoluteOrientation;
                double tgtOrientation = gWorld->getRobot(objectId-gRobotIndexStartOffset)->getWorldModel()->_agentAbsoluteOrientation;
                double delta_orientation = - ( srcOrientation - tgtOrientation );
                if ( delta_orientation >= 180.0 )
                    delta_orientation = - ( 360.0 - delta_orientation );
                else
                    if ( delta_orientation <= -180.0 )
                        delta_orientation = - ( - 360.0 - delta_orientation );
                (*inputs)[inputToUse] = delta_orientation/180.0;
                inputToUse++;
                
                /* //todelete
                 // ----- DEBUG::SANDBOX
                 std::cout << "src.orientation: " <<  srcOrientation
                 << "° ; tgt.orientation: " << tgtOrientation
                 << "° ; delta orientation: " << delta_orientation
                 << " <==> " << delta_orientation/180.0 << "°"
                 << std::endl;
                 // ----- DEBUG::SANDBOX.
                 */
            }
            else
            {
                (*inputs)[inputToUse] = 0; // not an agent...
                inputToUse++;
                (*inputs)[inputToUse] = 0; // ...therefore no match wrt. group.
                inputToUse++;
                /*
                 (*inputs)[inputToUse] = 0; // ...and no orientation.
                 inputToUse++;
                 */
            }
            
            // input: wall or empty?
            if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ) // not empty, but cannot be identified: this is a wall.
                (*inputs)[inputToUse] = 1;
            else
                (*inputs)[inputToUse] = 0; // nothing. (objectId=-1)
            inputToUse++;
        }
    }
    
    // floor sensor
    (*inputs)[inputToUse++] = (double)_wm->getGroundSensor_redValue()/255.0;
    (*inputs)[inputToUse++] = (double)_wm->getGroundSensor_greenValue()/255.0;
    (*inputs)[inputToUse++] = (double)_wm->getGroundSensor_blueValue()/255.0;
    
    // landmark (targeted landmark depends on g_skill)
    (*inputs)[inputToUse++] = _wm->getLandmarkDirectionAngleValue();
    (*inputs)[inputToUse++] = _wm->getLandmarkDistanceValue();

    
    // energy level
    if ( gEnergyLevel )
    {
        (*inputs)[inputToUse++] = _wm->getEnergyLevel() / gEnergyMax;
    }
    
    // ---- compute and read out ----
    
    nn->setWeigths(_parameters); // create NN
    
    nn->setInputs(*inputs);
    
    nn->step();
    
    std::vector<double> outputs = nn->readOut();
    
    // std::cout << "[DEBUG] Neural Network :" << nn->toString() << " of size=" << nn->getRequiredNumberOfWeights() << std::endl;
    
    _wm->_desiredTranslationalValue = outputs[0];
    _wm->_desiredRotationalVelocity = outputs[1];
    
    if ( TemplateMedeaSharedData::gEnergyRequestOutput )
    {
        _wm->setEnergyRequestValue(outputs[2]);
    }
    
    // normalize to motor interval values
    _wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue * gMaxTranslationalSpeed;
    _wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity * gMaxRotationalSpeed;
    
    delete (inputs);
}


void TemplateMedeaController::createNN()
{
    if ( nn != NULL ) // useless: delete will anyway check if nn is NULL or not.
        delete nn;
    
    switch ( TemplateMedeaSharedData::gControllerType )
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
            std::cerr << "[ERROR] gController type unknown (value: " << TemplateMedeaSharedData::gControllerType << ").\n";
            exit(-1);
    };
}


unsigned int TemplateMedeaController::computeRequiredNumberOfWeights()
{
    unsigned int res = nn->getRequiredNumberOfWeights();
    return res;
}

// ################ ######################## ################
// ################ ######################## ################
// ################ EVOLUTION ENGINE METHODS ################
// ################ ######################## ################
// ################ ######################## ################

void TemplateMedeaController::stepEvolution()
{
    
    // * broadcasting genome : robot broadcasts its genome to all neighbors (contact-based wrt proximity sensors)
    
    if ( _wm->isAlive() == true && gRadioNetwork )  	// only if agent is active (ie. not just revived) and deltaE>0.
    {
        broadcastGenome();
    }
    
    // * lifetime ended: replace genome (if possible)
    if( gWorld->getIterations() > 0 && gWorld->getIterations() % TemplateMedeaSharedData::gEvaluationTime == 0 )
                    //dynamic_cast<TemplateMedeaWorldObserver*>(gWorld->getWorldObserver())->getGenerationItCount() == 0 )  //todelete
    {
        loadNewGenome();
        _nbGenomeTransmission = 0;
    }
    else
    {
        _dSumTravelled = _dSumTravelled + getEuclidianDistance( _wm->getXReal(), _wm->getYReal(), _Xinit, _Yinit ); //remark: incl. squareroot.
    }
    
    // log the genome (only at the second iteration during snapshot time)
    if ( TemplateMedeaSharedData::gLogGenome && gWorld->getIterations() % ( TemplateMedeaSharedData::gEvaluationTime * TemplateMedeaSharedData::gSnapshotsFrequency ) == 1 )
    {
        // Logging: full genome
        std::string sLog = std::string("");
        sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",genome,";
        if ( _wm->isAlive() )
        {
            for(unsigned int i=0; i<_genome.size(); i++)
            {
                sLog += std::to_string(_genome[i]);
                if ( i < _genome.size()-1 )
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
        reset();
        setNewGenomeStatus(false);
    }
}


void TemplateMedeaController::selectRandomGenome()
{
    if(_genomesList.size() != 0)
    {
        int randomIndex = rand()%_genomesList.size();
        std::map<int, std::vector<double> >::iterator it = _genomesList.begin();
        while (randomIndex !=0 )
        {
            it ++;
            randomIndex --;
        }
        
        _currentGenome = (*it).second;
        
        // ## mutation scheme :: start
        
        // modified medea
        if ( TemplateMedeaSharedData::gIndividualMutationRate > rand()/RAND_MAX )
        {
            switch ( TemplateMedeaSharedData::gMutationOperator )
            {
                case 0:
                    mutateUniform();
                    break;
                case 1:
                    mutate(_sigmaList[(*it).first]); // vanilla MEDEA, used before year 2015
                    break;
                case 2:
                    mutate(TemplateMedeaSharedData::gSigma);
                    break;
            }
        }
        else
            _sigmaList[(*it).first];
        
        // ## mutation scheme :: end

        setNewGenomeStatus(true);
        
        _birthdate = gWorld->getIterations();
        
        // Logging: track descendance
        std::string sLog = std::string("");
        sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",descendsFrom," + std::to_string((*it).first) + "::" + std::to_string(_birthdateList[(*it).first]) + "\n";
        gLogManager->write(sLog);
        gLogManager->flush();
        
        _genomesList.clear();
    }
}

void TemplateMedeaController::selectFirstGenome()
{
    if(_genomesList.size() != 0)
    {
        _currentGenome = (*_genomesList.begin()).second;
        
        mutate(_sigmaList[(*_genomesList.begin()).first]);
        
        setNewGenomeStatus(true);
        
        _birthdate = gWorld->getIterations();
        
        // Logging: track descendance
        std::string sLog = std::string("");
        sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",descendsFrom," + std::to_string((*_genomesList.begin()).first) + "::" + std::to_string(_birthdateList[(*_genomesList.begin()).first]) + "\n";
        gLogManager->write(sLog);
        gLogManager->flush();
        
        _genomesList.clear();
    }
}

bool TemplateMedeaController::storeGenome(std::vector<double> genome, int senderId, int senderBirthdate, float sigma)
{
    std::map<int,int>::const_iterator it = _birthdateList.find(senderBirthdate);
    
    if ( !_isListening || ( it != _birthdateList.end() && _birthdateList[senderId] == senderBirthdate ) ) // this agent is not listening OR this exact agent's genome is already stored.
        return false;
    else
    {
        _genomesList[senderId] = genome;
        _sigmaList[senderId] = sigma;
        _birthdateList[senderId] = senderBirthdate;
        return true;
    }
}


void TemplateMedeaController::mutate( float sigma) // mutate within bounds.
{
    _genome.clear();
    
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
        
        _genome.push_back(value);
    }
    
    _currentGenome = _genome;
    
}


void TemplateMedeaController::mutateUniform() // mutate within bounds.
{
    _genome.clear();
    
    for (unsigned int i = 0 ; i != _currentGenome.size() ; i++ )
    {
        float randomValue = float(rand()%100) / 100.0; // in [0,1[
        double range = _maxValue - _minValue;
        double value = randomValue * range + _minValue;
        
        _genome.push_back(value);
    }
    
    _currentGenome = _genome;
    
}


void TemplateMedeaController::resetRobot()
{
    _nbInputs = 0;
    
    if ( gExtendedSensoryInputs ) // EXTENDED SENSORY INPUTS: code provided as example, should be rewritten to suit your need.
    {
        _nbInputs = ( PhysicalObjectFactory::getNbOfTypes()+3+1 ) * _wm->_cameraSensorsNb; // nbOfTypes + ( isItAnAgent? + isItSameGroupId? + agentAngleDifference?) + isItAWall?
    }
    
    _nbInputs += _wm->_cameraSensorsNb + 3; // proximity sensors + ground sensor (3 values)
    if ( gEnergyLevel )
        _nbInputs += 1; // incl. energy level
    if ( gLandmarks.size() > 0 )
        _nbInputs += 2; // incl. landmark (angle,dist)
    
    _nbOutputs = 2;
    if ( TemplateMedeaSharedData::gEnergyRequestOutput )
        _nbOutputs += 1; // incl. energy request
    
    _nbHiddenLayers = TemplateMedeaSharedData::gNbHiddenLayers;
    
    _nbNeuronsPerHiddenLayer = new std::vector<unsigned int>(_nbHiddenLayers);
    for(unsigned int i = 0; i < _nbHiddenLayers; i++)
        (*_nbNeuronsPerHiddenLayer)[i] = TemplateMedeaSharedData::gNbNeuronsPerHiddenLayer;
    
    createNN();
    
    unsigned int const nbGene = computeRequiredNumberOfWeights();
    
    if ( gVerbose )
        std::cout << std::flush ;
    
    _genome.clear();
    
    // Intialize genome
    
    for ( unsigned int i = 0 ; i != nbGene ; i++ )
    {
        _genome.push_back((double)(rand()%TemplateMedeaSharedData::gNeuronWeightRange)/(TemplateMedeaSharedData::gNeuronWeightRange/2)-1.0); // weights: random init between -1 and +1
    }
    
    // initialize robot
    
    _currentGenome = _genome;
    setNewGenomeStatus(true);
    _genomesList.clear();
    
}


void TemplateMedeaController::broadcastGenome()
{
    // remarque \todo: limiting genome transmission is sensitive to sensor order. (but: assume ok)
    
    for( int i = 0 ; i < _wm->_cameraSensorsNb && ( TemplateMedeaSharedData::gLimitGenomeTransmission == false || ( TemplateMedeaSharedData::gLimitGenomeTransmission == true && _nbGenomeTransmission < TemplateMedeaSharedData::gMaxNbGenomeTransmission ) ); i++)
    {
        int targetIndex = _wm->getObjectIdFromCameraSensor(i);
        
        if ( targetIndex >= gRobotIndexStartOffset && targetIndex < gRobotIndexStartOffset+gNumberOfRobots )   // sensor ray bumped into a robot : communication is possible
        {
            targetIndex = targetIndex - gRobotIndexStartOffset; // convert image registering index into robot id.
            
            TemplateMedeaController* targetRobotController = dynamic_cast<TemplateMedeaController*>(gWorld->getRobot(targetIndex)->getController());
            
            if ( ! targetRobotController )
            {
                std::cerr << "Error from robot " << _wm->getId() << " : the observer of robot " << targetIndex << " is not compatible." << std::endl;
                exit(-1);
            }
            
            if ( targetRobotController->isListening() )
            {
                float dice = float(rand()%100) / 100.0;
                float sigmaSendValue = _currentSigma;
                
                if ( dice <= TemplateMedeaSharedData::gProbaMutation )
                {
                    dice = float(rand() %100) / 100.0;
                    if ( dice < 0.5 )
                    {
                        sigmaSendValue = _currentSigma * ( 1 + TemplateMedeaSharedData::gUpdateSigmaStep ); // increase sigma
                        
                        if (sigmaSendValue > TemplateMedeaSharedData::gSigmaMax)
                        {
                            sigmaSendValue = TemplateMedeaSharedData::gSigmaMax;
                        }
                    }
                    else
                    {
                        sigmaSendValue = _currentSigma * ( 1 - TemplateMedeaSharedData::gUpdateSigmaStep ); // decrease sigma
                        
                        if ( sigmaSendValue < TemplateMedeaSharedData::gSigmaMin )
                        {
                            sigmaSendValue = TemplateMedeaSharedData::gSigmaMin;
                        }
                    }
                    
                }
                
                bool success = targetRobotController->storeGenome(_currentGenome, _wm->getId(), _birthdate, sigmaSendValue); // other agent stores my genome. Contaminant stragegy.
                
                if ( success == true )
                    _nbGenomeTransmission++;
            }
        }
    }
}

void TemplateMedeaController::loadNewGenome()
{
    if ( _wm->isAlive() || gEnergyRefill )  // ( gEnergyRefill == true ) enables revive
    {
        logCurrentState();
        
        // note: at this point, agent got energy, whether because it was revived or because of remaining energy.
        
        if (_genomesList.size() > 0)
        {
            // case: 1+ genome(s) imported, random pick.
            
            switch ( TemplateMedeaSharedData::gSelectionMethod )
            {
                case 0:
                    selectRandomGenome();
                    break;
                case 1:
                    selectFirstGenome();
                    break;
                default:
                    std::cerr << "[ERROR] unknown selection method (gSelectionMethod = " << TemplateMedeaSharedData::gSelectionMethod << ")\n";
                    exit(-1);
            }
            
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
                
                resetRobot(); // destroy then create a new NN
                
                _wm->setAlive(false); // inactive robot *must* import a genome from others (ie. no restart).
                
                if ( TemplateMedeaSharedData::gNotListeningStateDelay != 0 ) // ie. -1 (infinite,dead) or >0 (temporary,mute)
                {
                    _isListening = false;

                    _notListeningDelay = TemplateMedeaSharedData::gNotListeningStateDelay;
                    _listeningDelay = TemplateMedeaSharedData::gListeningStateDelay;
                    _wm->setRobotLED_colorValues(0, 0, 255); // is not listening
                    
                    std::string sLog = std::string("");
                    sLog += "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) + ",status,inactive\n";
                    gLogManager->write(sLog);
                    gLogManager->flush();
                    
                }
                else
                {
                    _listeningDelay = TemplateMedeaSharedData::gListeningStateDelay;

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

void TemplateMedeaController::logCurrentState()
{
    // Logging
    std::string sLog = "" + std::to_string(gWorld->getIterations()) + "," + std::to_string(_wm->getId()) + "::" + std::to_string(_birthdate) +
    ",age," + std::to_string(gWorld->getIterations()-_birthdate) +
    ",energy," +  std::to_string(_wm->getEnergyLevel()) +
    ",genomeList," + std::to_string(_genomesList.size()) +
    ",sigma," + std::to_string(_currentSigma) +
    ",x_init," + std::to_string(_wm->getXReal()) +
    ",y_init," + std::to_string(_wm->getYReal()) +
    ",x_current," + std::to_string(_Xinit) +
    ",y_current," + std::to_string(_Yinit) +
    ",dist," + std::to_string( getEuclidianDistance( _Xinit, _Yinit, _wm->getXReal(), _wm->getYReal() ) ) +
    ",sumOfDist," + std::to_string( _dSumTravelled ) +
    ",groupId," + std::to_string(_wm->getGroupId()) +
    "\n";
    gLogManager->write(sLog);
    gLogManager->flush();
}