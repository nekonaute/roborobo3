/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#include <neuralnetworks/NeuralNetwork.h>
#include <sstream>
#include <iostream>

using namespace Neural;



/* --------------------- NeuralNetwork --------------------- */

NeuralNetwork::~NeuralNetwork() {
	// ...
}

NeuralNetwork::NeuralNetwork(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs) :
		_weights(weights), _nbInputs(nbInputs), _nbOutputs(nbOutputs), _requiredNumberOfWeights(0),
		_haveInputsChanged(false), _haveOutputsChanged(false), _haveWeightsChanged(true) {
	_inputs = std::vector<double>(_nbInputs, 0.0);
	_outputs = std::vector<double>(_nbOutputs, 0.0);
}

NeuralNetwork::NeuralNetwork(NeuralNetwork const& other) {
	_nbInputs = other._nbInputs;
	_nbOutputs = other._nbOutputs;
	_inputs = other._inputs;
	_outputs = other._outputs;
	_weights = other._weights;
	_requiredNumberOfWeights = other._requiredNumberOfWeights;
	_haveInputsChanged = other._haveInputsChanged;
	_haveOutputsChanged = other._haveOutputsChanged;
	_haveWeightsChanged = other._haveWeightsChanged;
}

void NeuralNetwork::setDescription(std::string description) {
	_description = description;
}

std::string NeuralNetwork::getDescription() const {
	return _description;
}

void NeuralNetwork::setWeigths(std::vector<double>& weights) {
	_weights = weights;
	_haveWeightsChanged = true;
}

std::vector<double> const& NeuralNetwork::getWeigths() const {
	return _weights;
}

const std::vector<double>& NeuralNetwork::getInputs() const {
	return _inputs;
}

void NeuralNetwork::setInputs(std::vector<double>& inputs) {
	_inputs = inputs;
	_haveInputsChanged = true;
}

const std::vector<double>& NeuralNetwork::readOut() const {
	return _outputs;
}

void NeuralNetwork::setOutputs(std::vector<double>& outputs) {
	_outputs = outputs;
	_haveOutputsChanged = true;
}


std::string NeuralNetwork::toString() const {
	return _description;
}

std::ostream& operator<<(std::ostream& os, NeuralNetwork const& p) {
	return os << p.toString();
}

void NeuralNetwork::displayInfo() const {
	std::cout << toString();
}

void NeuralNetwork::writeLog() const {
	// ...
}

unsigned int NeuralNetwork::getNbInputs() const {
	return _nbInputs;
}

unsigned int NeuralNetwork::getNbOutputs() const {
	return _nbOutputs;
}

unsigned int NeuralNetwork::getRequiredNumberOfWeights() {
	if(_requiredNumberOfWeights == 0)
		_requiredNumberOfWeights = computeRequiredNumberOfWeights();
	return _requiredNumberOfWeights;
}

