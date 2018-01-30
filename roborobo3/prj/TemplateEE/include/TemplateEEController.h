/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef TEMPLATEEECONTROLLER_H
#define TEMPLATEEECONTROLLER_H

#include "RoboroboMain/common.h"
#include "Controllers/Controller.h"
#include <neuralnetworks/NeuralNetwork.h>
#include "RoboroboMain/common.h"

using namespace Neural;

class RobotWorldModel;

class TemplateEEController : public Controller
{
protected:
    int _iteration;
    int _birthdate; // evaluation when this controller was initialized.
        
    bool _isListening;
    int _notListeningDelay;
    int _listeningDelay;
    
    int _nbGenomeTransmission;
    
    std::vector<double> _parameters;
    std::string _nnType;
    std::vector<int> _nbHiddenNeuronsPerLayer;
    std::vector<int> _nbBiasNeuronsPerLayer;
    NeuralNetwork* nn;
    
    void createNN();
    
    //bool _isAlive; // agent stand still if not.
    bool _isNewGenome;
    
    void selectRandomGenome();
    void selectFirstGenome();
    void selectBestGenome();
    void selectFitProp();
    
    void mutateGaussian( float sigma );
    void mutateUniform();
    
    void mutateSigmaValue();
    
    std::vector<double> getInputs();
    void setIOcontrollerSize();
    
    virtual void initController(); // called by resetRobot
    virtual void stepController();
    
    void stepEvolution();
    
    virtual void broadcastGenome();
    
    void loadNewGenome();
    
    unsigned int computeRequiredNumberOfWeights();
    
    //        void setAliveStatus( bool isAlive ) { _isAlive = isAlive; }
    bool getNewGenomeStatus() { return _isNewGenome; }
    void setNewGenomeStatus( bool __status ) { _isNewGenome = __status; }
    
    // incoming genomes reservoir
    
    std::map< std::pair<int,int>, std::vector<double> > _genomesList;
    std::map< std::pair<int,int>, float > _sigmaList;
    std::map< std::pair<int,int>, float > _fitnessValuesList;
    
    // current genome
    
    std::vector<double> _currentGenome;
    float _currentSigma;
    
    // ANN
    double _minValue;
    double _maxValue;
    unsigned int _nbInputs;
    unsigned int _nbOutputs;
    unsigned int _nbHiddenLayers;
    std::vector<unsigned int>* _nbNeuronsPerHiddenLayer;
    
    // logging purpose
    double _Xinit;
    double _Yinit;
    double _dSumTravelled;
    
    virtual bool storeGenome(std::vector<double> genome, std::pair<int,int> senderId, float sigma, float fitness=0);
    void reset() override;
    
    void clearReservoir(); // clear genomesList, sigmaList, fitnessesList and birthdayList
    
    virtual void logCurrentState();
    
    virtual void mapGenotypeToPhenotype();
    
    virtual void performSelection();
    virtual void performVariation();
    
    virtual void resetFitness();
    virtual void updateFitness();
    
public:
    
    TemplateEEController(RobotWorldModel *wm);
    ~TemplateEEController();
    
    void step() override;
    
    int getBirthdate() { return _birthdate; }
    
    bool isListening() { return _isListening; }
    
    // Remarks on fitness value:
    // the current fitness is transmitted to other nearby robots.
    // in case of multiple encounters between robots/genomes (ie. the robot still bears the same genome)  
    // selection will then act using fitness which can be taken at very different moment (e.g. comparing a very good but young genome with a not-so-good but old genome. While the younger will very probably end up with a better fitness score, the old one is advantaged by the total amount of time spent -- e.g. with a foraging task.)

    virtual double getFitness();
    
    int getGenomeReservoirSize() { return (int)_genomesList.size(); }

    
};


#endif

