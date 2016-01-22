/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#include <neuralnetworks/Elman.h>
#include <sstream>
#include <math.h>

using namespace Neural;


/* --------------------- Elman --------------------- */

Elman::Elman(std::vector<double>& weights,
		unsigned int nbInputs,
		unsigned int nbOutputs,
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		MLP(weights, nbInputs, nbOutputs, activeBiais, onlyUseBiaisForFirstHiddenLayer, biaisValue) {
	initLastOutputs();
}


Elman::Elman(std::vector<double>& weights,
		unsigned int nbInputs,
		unsigned int nbOutputs,
		std::vector<unsigned int>& nbNeuronsPerLayer,
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		MLP(weights, nbInputs, nbOutputs, nbNeuronsPerLayer, activeBiais, onlyUseBiaisForFirstHiddenLayer, biaisValue) {
	initLastOutputs();
}



Elman::Elman(Elman const& other) : MLP(other) {
	_lastOutputs = other._lastOutputs;
}

Elman::~Elman() {
	// ...
}


std::vector< std::vector<double> > const& Elman::getLastOutputs() const {
	return _lastOutputs;
}


std::string Elman::toString() const {
	std::stringstream ss;
	ss << "nn(";

	ss << _nbInputs;
	if(_activeBiais)
		ss << "(+1)";

	for(size_t i = 1; i < _nbNeuronsPerLayer.size(); i++) {
		ss << ";";
		ss << _nbNeuronsPerLayer[i];
		ss << "[";
		int nbConnexions = _nbNeuronsPerLayer[i - 1];
		if(i != _nbNeuronsPerLayer.size() - 1)
			nbConnexions += _nbNeuronsPerLayer[i];
		if(_activeBiais && !_onlyUseBiaisForFirstHiddenLayer)
			++nbConnexions;
		ss << nbConnexions;
		ss << "]";
	}

	ss << ")";
	return ss.str();
}


void Elman::initLastOutputs() {
	_lastOutputs = std::vector< std::vector<double> >(_nbNeuronsPerLayer.size() - 2);
	for(size_t i = 0; i < _lastOutputs.size(); i++) {
		_lastOutputs[i] = std::vector<double>(_nbNeuronsPerLayer[i + 1]);
		for(size_t j = 0; j < _nbNeuronsPerLayer[i + 1]; j++)
			_lastOutputs[i][j] = 0.0;
	}
}


void Elman::step() {
    
	std::vector<double> prec = _inputs;
	unsigned int weightsIndice = 0;
	std::vector<double> tmp;

	// Verify that the number of layers is correct
	if(_nbNeuronsPerLayer.size() < 2)
		throw NeuralNetworkException("MLP must have at least 2 layers : input and output");
	if(_nbNeuronsPerLayer[0] != _inputs.size())
		throw NeuralNetworkException("nbNeuronsPerLayer has an incorrect number of inputs neurons (first layer)");
	if(_nbNeuronsPerLayer[_nbNeuronsPerLayer.size() - 1] == 0)
		throw NeuralNetworkException("nbNeuronsPerLayer has an incorrect number of output neurons (output layer)");
	if(_nbNeuronsPerLayer.size() != _lastOutputs.size() + 2 )
		throw NeuralNetworkException("lastOutputs has an incorrect number of entries");

	unsigned int nbBiais = 0;
	if(_activeBiais)
		nbBiais = 1;

	for(unsigned int k = 0; k < _nbNeuronsPerLayer.size() - 1; k++) {

		// Verify that the number of weights is correct
		unsigned int const remainingWeights = (int)(_weights.size() - weightsIndice);
		if(remainingWeights < prec.size() + nbBiais) {
			std::stringstream ss;
			ss << "remainingWeights = " << remainingWeights <<  
					" but at least " << prec.size() + nbBiais << " weights are needed...";
			throw NeuralNetworkException(ss.str());
		}

		unsigned int nbOutputs = _nbNeuronsPerLayer[k + 1];

		// Do Stuffs ! (base neural computation)
		tmp = std::vector<double>(nbOutputs);
		for(unsigned int i = 0; i < prec.size(); i++) {
			for(unsigned int j = 0; j < nbOutputs; j++) {
				tmp[j] += prec[i] * _weights[weightsIndice++];
			}
		}

		// Do computation of the biais
		if(_activeBiais && (k==0 || !_onlyUseBiaisForFirstHiddenLayer))
			for(unsigned int j = 0; j < nbOutputs; j++)
				tmp[j] += tanh(_weights[weightsIndice++]) * _biaisValue;

		// Do computation using last outputs of this layer
		if(k < _nbNeuronsPerLayer.size() - 2)
			for(unsigned int i = 0; i < _lastOutputs[k].size(); i++) {
				for(unsigned int j = 0; j < nbOutputs; j++) {
					tmp[j] += _lastOutputs[k][i] * _weights[weightsIndice++];
				}
			}

		// Tanh activation function
		for(unsigned int i = 0; i < nbOutputs; i++)
			tmp[i] = tanh(tmp[i]);

		prec = tmp;

		if(k < _nbNeuronsPerLayer.size() - 2)
			_lastOutputs[k] = tmp;
	}

	_outputs = tmp;
}

unsigned int Elman::computeRequiredNumberOfWeights() {
	size_t res = MLP::computeRequiredNumberOfWeights();
	for(size_t i = 1; i < _nbNeuronsPerLayer.size() - 1; i++)
		res += _nbNeuronsPerLayer[i] * _nbNeuronsPerLayer[i];
	return (unsigned int)res;
}

std::string Elman::getNNTypeName() {
	return "Elman";
}


Elman* Elman::clone() const {
	return new Elman(*this);
}

