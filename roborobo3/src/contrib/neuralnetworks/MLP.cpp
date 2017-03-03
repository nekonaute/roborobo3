/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#include <neuralnetworks/MLP.h>
#include <neuralnetworks/NeuralNetwork.h>
#include <sstream>
#include <math.h>

using namespace Neural;


/* --------------------- MLP --------------------- */

MLP::MLP(std::vector<double>& weights,
		unsigned int nbInputs,
		unsigned int nbOutputs,
		bool activeBias,
		bool onlyUseBiasForFirstHiddenLayer,
		double biasValue) :
		LayeredNeuralNetwork(weights, nbInputs, nbOutputs, activeBias, onlyUseBiasForFirstHiddenLayer, biasValue) {
	// ...
}


MLP::MLP(std::vector<double>& weights,
		unsigned int nbInputs,
		unsigned int nbOutputs,
		std::vector<unsigned int>& nbNeuronsPerLayer,
		bool activeBias,
		bool onlyUseBiasForFirstHiddenLayer,
		double biasValue) :
		LayeredNeuralNetwork(weights, nbInputs, nbOutputs, nbNeuronsPerLayer, activeBias, onlyUseBiasForFirstHiddenLayer, biasValue) {
	// ...
}



MLP::MLP(MLP const& other) : LayeredNeuralNetwork(other) {
	// ...
}

MLP::~MLP() {
	// ...
}


std::string MLP::toString() const {
	return LayeredNeuralNetwork::toString();
}


void MLP::step() {

	std::vector<double> prec = _inputs;
	unsigned int weightsIndice = 0;
	std::vector<double> tmp;

	// Verify that the number of layers is correct
//	if(_nbNeuronsPerLayer.size() < 2)
//		throw NeuralNetworkException("MLP must have at least 2 layers : input and output");
//	if(_nbNeuronsPerLayer[0] != _inputs.size())
//		throw NeuralNetworkException("nbNeuronsPerLayer has an incorrect number of inputs neurons (first layer)");
//	if(_nbNeuronsPerLayer[_nbNeuronsPerLayer.size() - 1] == 0)
//		throw NeuralNetworkException("nbNeuronsPerLayer has an incorrect number of output neurons (output layer)");

//	unsigned int nbBias = 0;
//	if(_activeBias)
//		nbBias = 1;

	for(unsigned int k = 0; k < _nbNeuronsPerLayer.size() - 1; k++) {

		unsigned int const nbOutputs = _nbNeuronsPerLayer[k + 1];

		// Do Stuffs ! (base neural computation)
		tmp = std::vector<double>(nbOutputs, 0.0);
		for(unsigned int i = 0; i < prec.size(); i++) {
			for(unsigned int j = 0; j < nbOutputs; j++) {
				tmp[j] += prec[i] * _weights[weightsIndice++];
			}
		}

		// Do computation of the bias
		if(_activeBias && (k==0 || !_onlyUseBiasForFirstHiddenLayer))
			for(unsigned int j = 0; j < nbOutputs; j++)
				//tmp[j] += tanh(_weights[weightsIndice++]) * .08;
				tmp[j] +=  _biasValue * _weights[weightsIndice++];

		// Tanh activation function
		for(unsigned int i = 0; i < nbOutputs; i++)
			tmp[i] = tanh(tmp[i]);

		prec = tmp;
	}

	_outputs = tmp;
}


unsigned int MLP::computeRequiredNumberOfWeights() {
	unsigned int res = 0;
	unsigned int nbBias = 0;
	if(_activeBias)
		nbBias = 1;

	if(_nbNeuronsPerLayer.size() <= 2) {
		return (_nbInputs + nbBias) * _nbOutputs;
	} else {
		res += (_nbInputs + nbBias) * _nbNeuronsPerLayer[1];
		if(_onlyUseBiasForFirstHiddenLayer)
			nbBias = 0;
		for(size_t i = 1; i < _nbNeuronsPerLayer.size() - 1; i++) {
			res += (_nbNeuronsPerLayer[i] + nbBias) * _nbNeuronsPerLayer[i + 1];
		}
		return res;
	}
}


std::string MLP::getNNTypeName() {
	return "MLP";
}

MLP* MLP::clone() const {
	return new MLP(*this);
}

