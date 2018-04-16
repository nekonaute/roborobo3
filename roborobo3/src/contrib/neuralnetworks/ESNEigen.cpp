/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#include <neuralnetworks/ESNEigen.h>
#include <neuralnetworks/NeuralNetwork.h>
#include <sstream>
#include <memory>
#include <random>

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>
#include <Eigen/LU>


using namespace Neural;



/* --------------------- ESNEigen --------------------- */

ESNEigen::ESNEigen(std::vector<real_t>& weights, size_t nbInputs, size_t nbOutputs, size_t reservoirSize, real_t densityOfConnections, real_t alpha,
		seed_t seed,
		real_t inputToReservoirWeightRange,
		real_t inputToOutputWeightRange,
		real_t reservoirToOutputWeightRange,
		real_t outputToReservoirWeightRange,
		real_t outputToOutputWeightRange,
		bool allowInputToOutputDirectConnections,
		bool allowOutputSelfRecurrentConnections,
		bool allowInputToReservoirConnections,
		bool fixedInputToReservoirConnections,
		bool allowOutputToReservoirConnections,
		bool addConstantInputBias,
		bool addSinInputBias,
		real_t sinBiasPeriod,
		bool useSparseComputation) :
		ReservoirNeuralNetwork(weights, nbInputs, nbOutputs, reservoirSize, inputToReservoirWeightRange, inputToOutputWeightRange, reservoirToOutputWeightRange, outputToReservoirWeightRange, outputToOutputWeightRange, allowInputToOutputDirectConnections, allowOutputSelfRecurrentConnections, allowInputToReservoirConnections, allowOutputToReservoirConnections), _densityOfConnections(densityOfConnections), _alpha(alpha), _seed(seed), _fixedInputToReservoirConnections(fixedInputToReservoirConnections), _addConstantInputBias(addConstantInputBias), _addSinInputBias(addSinInputBias), _sinBiasPeriod(sinBiasPeriod), _useSparseComputation(useSparseComputation) {

	createESN(seed);
}

ESNEigen::ESNEigen(ESNEigen const& other) : ReservoirNeuralNetwork(other) {
	_w = other._w;
	_wSparse = other._wSparse;
	_neuronsValues = other._neuronsValues;
	_newValues = other._newValues;
	_densityOfConnections = other._densityOfConnections;
	_alpha = other._alpha;
	_seed = other._seed;
	_fixedInputToReservoirConnections = other._fixedInputToReservoirConnections;
	_nbAdditionalInputs = other._nbAdditionalInputs;
	_addConstantInputBias = other._addConstantInputBias;
	_addSinInputBias = other._addSinInputBias;
	_sinBiasPeriod = other._sinBiasPeriod;
	_sinBiasParam = other._sinBiasParam;
}


size_t ESNEigen::getNbNeurons() {
	return _nbInputs + _nbAdditionalInputs + _nbOutputs + _reservoirSize;
}


void ESNEigen::createESN(seed_t const& seed) {
	// Update _nbAdditionalInputs to take biases into account
	_nbAdditionalInputs = 0;
	if(_addConstantInputBias)
		++_nbAdditionalInputs;
	if(_addSinInputBias)
		++_nbAdditionalInputs;
	size_t const nbTotalInputs = _nbInputs + _nbAdditionalInputs;

	// Define some constants, for readability
	size_t const nbNeurons = getNbNeurons();
	size_t const startInput = getStartInput();
	size_t const startReservoir = getStartReservoir();
	size_t const startOutput = getStartOutput();


	// Initialise random number generation
	//std::random_device rd(static_cast<long unsigned int>(seed));
	//std::mt19937 mtRd(rd());
	std::default_random_engine mtRd(static_cast<long unsigned int>(seed));

	// Create weights matrix
	//_w = Eigen::SparseMatrix<real_t>(nbNeurons, nbNeurons);
	_w = Eigen::MatrixXd(nbNeurons, nbNeurons);

	// Create neurons values
	_neuronsValues = Eigen::VectorXd(nbNeurons);
	_newValues = Eigen::VectorXd(nbNeurons);

	// Fill the matrix and vectors with zeroes (should be done !)
	_w.setZero();
	_neuronsValues.setZero();
	_newValues.setZero();

	// Create inputs to reservoir, if allowed
	if(_allowInputToReservoirConnections) {
		std::uniform_real_distribution<real_t> distr1(-_inputToReservoirWeightRange, _inputToReservoirWeightRange);
		for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
			for(size_t j = startInput; j < startInput + nbTotalInputs; j++)
				_w(i, j) = distr1(mtRd);
	}

	// Create inputs to outputs, if allowed
	if(_allowInputToOutputDirectConnections) {
		std::uniform_real_distribution<real_t> distr2(-_inputToOutputWeightRange, _inputToOutputWeightRange);
		for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
			for(size_t j = startInput; j < startInput + nbTotalInputs; j++)
				_w(i, j) = distr2(mtRd);
	}

	// Create outputs self recurrent connections, if allowed
	if(_allowOutputSelfRecurrentConnections) {
		std::uniform_real_distribution<real_t> distr3(-_outputToOutputWeightRange, _outputToOutputWeightRange);
		for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
			for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
				_w(i, j) = distr3(mtRd);
	}

	// Create outputs to reservoir, if allowed
	if(_allowOutputToReservoirConnections) {
		std::uniform_real_distribution<real_t> distr4(-_outputToReservoirWeightRange, _outputToReservoirWeightRange);
		for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
			for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
				_w(i, j) = distr4(mtRd);
	}

	// Create reservoir to outputs
	std::uniform_real_distribution<real_t> distr5(-1.0, 1.0);
	for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
		for(size_t j = startReservoir; j < startReservoir + _reservoirSize; j++)
			_w(i, j) = distr5(mtRd);

	// --- Create reservoir ---
	// Build connectivity matrix
	std::uniform_real_distribution<real_t> distr6(0.0, 1.0);
	for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
		for(size_t j = startReservoir; j < startReservoir + _reservoirSize; j++) {
			if(distr6(mtRd) < _densityOfConnections) {
				if(distr6(mtRd) < 0.5)
					_w(i, j) = 1.0;
				else
					_w(i, j) = -1.0;
			} else {
				_w(i, j) = 0.0;
			}
		}

	// Get spectral radius
	auto matReservoir = _w.block(startReservoir, startReservoir, _reservoirSize, _reservoirSize);
	decltype(_w) w2(_reservoirSize, _reservoirSize);
	Eigen::SelfAdjointEigenSolver<decltype(_w)> solver(matReservoir);
	real_t const spectralRadius = std::abs(solver.eigenvalues()[_reservoirSize - 1]);
	if(spectralRadius == 0)
		throw "Spectral radius for ESN cannot be 0. Setting higher density may help.";
	// Scale and damp weights
	w2 = matReservoir * (1. / spectralRadius);
	_w.block(startReservoir, startReservoir, _reservoirSize, _reservoirSize) = _alpha * w2;

	// Create sparse representation
	if(_useSparseComputation) {
		_wSparse = _w.sparseView();
	}

	//_w = Eigen::MatrixXd::Random(nbNeurons, nbNeurons);
	//Eigen::IOFormat heavyFmt(Eigen::FullPrecision, 0, ", ", ";\n", "[", "]", "[", "]");
	//Eigen::IOFormat commaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", " << ", ";");
	//std::cout << "[DEBUG] ESN4: " << _w.format(commaInitFmt) << std::endl;
	//std::cout << "[DEBUG] ESN5: " << computeRequiredNumberOfWeights() << std::endl;
}


void ESNEigen::step(size_t nbOfSteps) {
	// Define some constants, for readability
	size_t const nbNeurons = getNbNeurons();
	size_t const startInput = getStartInput();
	size_t const startReservoir = getStartReservoir();
	size_t const startOutput = getStartOutput();
	size_t const nbTotalInputs = _nbInputs + _nbAdditionalInputs;

	// Fill the vector with zeroes (should be done !)
	_newValues.setZero();

	// Update connection weights
	if(_useSparseComputation) {
		size_t weightsIndice = 0;
		if(_allowInputToReservoirConnections && !_fixedInputToReservoirConnections) {
			for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
				for(size_t j = startInput; j < startInput + nbTotalInputs; j++)
					_wSparse.coeffRef(i, j) = _weights[weightsIndice++];
		}
		if(_allowInputToOutputDirectConnections) {
			for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
				for(size_t j = startInput; j < startInput + nbTotalInputs; j++)
					_wSparse.coeffRef(i, j) = _weights[weightsIndice++];
		}
		if(_allowOutputSelfRecurrentConnections) {
			for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
				for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
					_wSparse.coeffRef(i, j) = _weights[weightsIndice++];
		}
		if(_allowOutputToReservoirConnections) {
			for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
				for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
					_wSparse.coeffRef(i, j) = _weights[weightsIndice++];
		}
	} else {
		size_t weightsIndice = 0;
		if(_allowInputToReservoirConnections && !_fixedInputToReservoirConnections) {
			for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
				for(size_t j = startInput; j < startInput + nbTotalInputs; j++)
					_w(i, j) = _weights[weightsIndice++];
		}
		if(_allowInputToOutputDirectConnections) {
			for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
				for(size_t j = startInput; j < startInput + nbTotalInputs; j++)
					_w(i, j) = _weights[weightsIndice++];
		}
		if(_allowOutputSelfRecurrentConnections) {
			for(size_t i = startOutput; i < startOutput + _nbOutputs; i++)
				for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
					_w(i, j) = _weights[weightsIndice++];
		}
		if(_allowOutputToReservoirConnections) {
			for(size_t i = startReservoir; i < startReservoir + _reservoirSize; i++)
				for(size_t j = startOutput; j < startOutput + _nbOutputs; j++)
					_w(i, j) = _weights[weightsIndice++];
		}
	}


	//Eigen::IOFormat commaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", " << ", ";");
	//std::cout << "[DEBUG] ESN1: " << _neuronsValues.format(commaInitFmt) << std::endl;
	//std::cout << "[DEBUG] ESN0: " << startInput << " " << startReservoir << " " << startOutput << std::endl;

	for(size_t i = 0; i < nbOfSteps; ++i) {

		// Put inputs values into neuronsValues
		for(size_t i = 0; i < _nbInputs; i++)
			_neuronsValues(startInput + i) = _inputs[i];
		// Add biases, if necessary
		size_t inputsIndice = _nbInputs;
		if(_addConstantInputBias) {
			_neuronsValues(startInput + inputsIndice) = 1.0;
			++inputsIndice;
		}
		if(_addSinInputBias) {
			_neuronsValues(startInput + inputsIndice) = std::sin(_sinBiasParam);
			//std::cout << "[DEBUG] ESN-Bias1: " << _neuronsValues(startInput + inputsIndice) << std::endl;
			_sinBiasParam += _sinBiasPeriod;
			++inputsIndice;
		}

		// Put outputs values into neuronsValues
		for(size_t i = 0; i < _nbOutputs; i++)
			_neuronsValues(startOutput + i) = _outputs[i];

		// Update neuron values
		if(_useSparseComputation) {
			Eigen::VectorXd tmp = _wSparse * _neuronsValues;
			//std::cout << "[DEBUG] ESN2: " << tmp.format(commaInitFmt) << std::endl;
			_newValues = tmp.array().tanh();
		} else {
			//_newValues = Eigen::tanh(_w * _neuronsValues);
			Eigen::VectorXd tmp = _w * _neuronsValues;
			//std::cout << "[DEBUG] ESN2: " << tmp.format(commaInitFmt) << std::endl;
			_newValues = tmp.array().tanh();
		}
		_neuronsValues = _newValues;

		// Update outputs values
		for(size_t i = 0; i < _nbOutputs; i++)
			_outputs[i] = _neuronsValues(startOutput + i);
		//std::cout << "[DEBUG] ESN3: " << _neuronsValues.format(commaInitFmt) << std::endl;
	}

}

void ESNEigen::step() {
	step(1);
}


unsigned int ESNEigen::computeRequiredNumberOfWeights() {
	size_t const nbTotalInputs = _nbInputs + _nbAdditionalInputs;
	size_t result = 0;
	result += _reservoirSize * _nbOutputs; // Reservoir --> output connexions

	if(_allowInputToReservoirConnections && !_fixedInputToReservoirConnections)
		result += nbTotalInputs * _reservoirSize;
	if(_allowInputToOutputDirectConnections)
		result += nbTotalInputs * _nbOutputs;
	if(_allowOutputSelfRecurrentConnections)
		result += _nbOutputs * _nbOutputs;
	if(_allowOutputToReservoirConnections)
		result += _nbOutputs * _reservoirSize;

	return (int)result;
}



