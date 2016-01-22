/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 * NNlib: Leo Cazenille <leo.cazenille@upmc.fr>
 */



#ifndef TEMPLATEMEDEACONTROLLER_H
#define TEMPLATEMEDEACONTROLLER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/Graphics.h"
#include "Controllers/Controller.h"
#include "WorldModels/RobotWorldModel.h"
#include "TemplateMedea/include/TemplateMedeaAgentObserver.h"
#include <neuralnetworks/NeuralNetwork.h>

#include <iomanip>

using namespace Neural;


class TemplateMedeaController : public Controller
{
private:
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
    
    void mutate(float sigma);
    void mutateUniform();
    
    void stepBehaviour();
    void stepEvolution();
    
    void broadcastGenome();
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
    
    bool storeGenome(std::vector<double> genome, int senderId, int senderBirthdate, float sigma);
    void resetRobot();
    
    void logCurrentState();
    
public:
    
    TemplateMedeaController(RobotWorldModel *wm);
    ~TemplateMedeaController();
    
    void reset();
    void step();
    
    int getBirthdate() { return _birthdate; }
    
    bool isListening() { return _isListening; }
    
};


#endif

