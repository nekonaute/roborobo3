/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#include <neuralnetworks/ReservoirNeuralNetwork.h>
#include <sstream>
#include <iostream>

using namespace Neural;


/* --------------------- ReservoirNeuralNetwork --------------------- */

ReservoirNeuralNetwork::~ReservoirNeuralNetwork() {
	// ...
}

ReservoirNeuralNetwork::ReservoirNeuralNetwork(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, unsigned int reservoirSize,
	double inputToReservoirWeightRange,
	double inputToOutputWeightRange,
	double reservoirToOutputWeightRange,
	double outputToReservoirWeightRange,
	double outputToOutputWeightRange,
	bool allowInputToOutputDirectConnections,
	bool allowOutputSelfRecurrentConnections,
	bool allowInputToReservoirConnections,
	bool allowOutputToReservoirConnections) :
		NeuralNetwork(weights, nbInputs, nbOutputs),
		_reservoirSize(reservoirSize),
		_inputToReservoirWeightRange(inputToReservoirWeightRange),
		_inputToOutputWeightRange(inputToOutputWeightRange),
		_reservoirToOutputWeightRange(reservoirToOutputWeightRange),
		_outputToReservoirWeightRange(outputToReservoirWeightRange),
		_outputToOutputWeightRange(outputToOutputWeightRange),
		_allowInputToOutputDirectConnections(allowInputToOutputDirectConnections),
		_allowOutputSelfRecurrentConnections(allowOutputSelfRecurrentConnections),
		_allowInputToReservoirConnections(allowInputToReservoirConnections),
		_allowOutputToReservoirConnections(allowOutputToReservoirConnections) {
	if(reservoirSize == 0)
		throw "reservoirSize cannot be 0";
}


ReservoirNeuralNetwork::ReservoirNeuralNetwork(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, unsigned int reservoirSize) :
		NeuralNetwork(weights, nbInputs, nbOutputs),
		_reservoirSize(reservoirSize),
		_inputToReservoirWeightRange(1.0),
		_inputToOutputWeightRange(1.0),
		_reservoirToOutputWeightRange(1.0),
		_outputToReservoirWeightRange(1.0),
		_outputToOutputWeightRange(1.0),
		_allowInputToOutputDirectConnections(true),
		_allowOutputSelfRecurrentConnections(false),
		_allowInputToReservoirConnections(true),
		_allowOutputToReservoirConnections(false) {
	if(reservoirSize == 0)
		throw "reservoirSize cannot be 0";
}



ReservoirNeuralNetwork::ReservoirNeuralNetwork(ReservoirNeuralNetwork const& other) : NeuralNetwork(other) {
	_reservoirSize = other._reservoirSize;

	_allowInputToOutputDirectConnections = other._allowInputToOutputDirectConnections;
	_allowOutputSelfRecurrentConnections = other._allowOutputSelfRecurrentConnections;
	_allowInputToReservoirConnections = other._allowInputToReservoirConnections;
	_allowOutputToReservoirConnections = other._allowOutputToReservoirConnections;

	_inputToReservoirWeightRange = other._inputToReservoirWeightRange;
	_inputToOutputWeightRange = other._inputToOutputWeightRange;
	_reservoirToOutputWeightRange = other._reservoirToOutputWeightRange;
	_outputToReservoirWeightRange = other._outputToReservoirWeightRange;
	_outputToOutputWeightRange = other._outputToOutputWeightRange;
}


unsigned int ReservoirNeuralNetwork::getReservoirSize() const {
	return _reservoirSize;
}


std::string ReservoirNeuralNetwork::toString() const {
	return NeuralNetwork::toString();
}


size_t ReservoirNeuralNetwork::getNbNeurons() {
	return _nbInputs + _nbOutputs + _reservoirSize;
}



bool ReservoirNeuralNetwork::areInputToOutputDirectConnectionsAllowed() const {
	return _allowInputToOutputDirectConnections;
}

bool ReservoirNeuralNetwork::areOutputSelfRecurrentConnectionsAllowed() const {
	return _allowOutputSelfRecurrentConnections;
}

bool ReservoirNeuralNetwork::areInputToReservoirConnectionsAllowed() const {
	return _allowInputToReservoirConnections;
}

bool ReservoirNeuralNetwork::areOutputToReservoirConnectionsAllowed() const {
	return _allowOutputToReservoirConnections;
}

