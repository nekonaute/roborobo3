/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef TEMPLATEEECONTROLLER_H
#define TEMPLATEEECONTROLLER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/Graphics.h"
#include "Controllers/Controller.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateEE/include/TemplateEEAgentObserver.h"
#include <neuralnetworks/NeuralNetwork.h>

#include <iomanip>

using namespace Neural;


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
    std::vector<int> _nbBiaisNeuronsPerLayer;
    NeuralNetwork* nn;
    
    void createNN();
    
    //bool _isAlive; // agent stand still if not.
    bool _isNewGenome;
    
    void selectRandomGenome();
    void selectFirstGenome();
    
    void mutateGaussian( float sigma );
    void mutateUniform();
    
    void mutateSigmaValue();
    
    std::vector<double> getInputs();
    void setIOcontrollerSize();
    
    virtual void stepController();
    void stepEvolution();
    
    virtual void broadcastGenome();
    
    void loadNewGenome();
    
    unsigned int computeRequiredNumberOfWeights();
    
    //        void setAliveStatus( bool isAlive ) { _isAlive = isAlive; }
    bool getNewGenomeStatus() { return _isNewGenome; }
    void setNewGenomeStatus( bool __status ) { _isNewGenome = __status; }
    
    // evolutionary engine
    std::vector<double> _genome; // todo: accessing
    std::map<int, std::vector<double> > _genomesList;
    std::map<int, float > _sigmaList;
    std::map<int,int> _birthdateList; // store the birthdate of the received controllers (useful for monitoring).
    std::map<int, float> _fitnessValuesList;
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
    
    bool storeGenome(std::vector<double> genome, int senderId, int senderBirthdate, float sigma, float fitness=0);
    void resetRobot();
    
    virtual void initController(); // called by resetRobot
    
    virtual void logCurrentState();
    
    virtual void performSelection();
    virtual void performVariation();
    
    virtual double getFitness();
    virtual void resetFitness( );
    virtual void updateFitness();
    
public:
    
    TemplateEEController(RobotWorldModel *wm);
    ~TemplateEEController();
    
    void step();
    
    int getBirthdate() { return _birthdate; }
    
    bool isListening() { return _isListening; }
    
};


#endif

