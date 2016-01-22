/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#include <neuralnetworks/Perceptron.h>
#include <sstream>
#include <math.h>

using namespace Neural;


/* --------------------- Perceptron --------------------- */


Perceptron::Perceptron(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs) :
		MLP(weights, nbInputs, nbOutputs, false) {
	_nbNeuronsPerLayer = std::vector<unsigned int>(2);
	_nbNeuronsPerLayer[0] = _nbInputs;
	_nbNeuronsPerLayer[1] = _nbOutputs;
}



Perceptron::Perceptron(Perceptron const& other) : MLP(other) {
	// ...
}

Perceptron::~Perceptron() {
	// ...
}

std::string Perceptron::toString() const {
	return LayeredNeuralNetwork::toString();
}


void Perceptron::step() {
	MLP::step();
}


unsigned int Perceptron::computeRequiredNumberOfWeights() {
	return MLP::computeRequiredNumberOfWeights();
}

std::string Perceptron::getNNTypeName() {
	return "Perceptron";
}

Perceptron* Perceptron::clone() const {
	return new Perceptron(*this);
}

