/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#ifndef ESNEIGEN_H
#define ESNEIGEN_H

#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/LU>


#include <neuralnetworks/ReservoirNeuralNetwork.h>
#include <string>
#include <vector>


namespace Neural {

	/**
	 * A support for ESN neural networks using the Eigen3 library
	 * @author Leo Cazenille <leo.cazenille@gmail.com>
	 */
	class ESNEigen : public ReservoirNeuralNetwork {
		public:
			typedef double real_t;
			typedef long unsigned int seed_t;

		protected:
			/** Weights Matrix */
			Eigen::MatrixXd _w;

			/** Sparse version of weights matrix */
			Eigen::SparseMatrix<real_t> _wSparse;

			/** Values of each neurons */
			Eigen::VectorXd _neuronsValues;

			/** New values of each neurons */
			Eigen::VectorXd _newValues;

			/** Density of connections */
			real_t _densityOfConnections;

			/** Reservoir's alpha parameter */
			real_t _alpha;

			/** Random seed used to create the reservoir */
			seed_t _seed;

			/** Whether input to reservoir connections are fixed or parametrised */
			size_t _fixedInputToReservoirConnections;

			/** Number of additional inputs (e.g. biais) */
			size_t _nbAdditionalInputs;

			/** Add a constant bias input (value=1.0) */
			bool _addConstantInputBias;

			/** Add a sinusoidal bias input */
			bool _addSinInputBias;

			/** Quantity added to sinusoidal bias input at each step */
			real_t _sinBiasPeriod;

			/** Current parameter of sinusoidal bias input */
			real_t _sinBiasParam = 0.0;

			/** Whether to use sparse or dense computation */
			bool _useSparseComputation = false;


			/**
			 * Create base stuffs of the ESN
			 */
			void createESN(seed_t const& seed);

			/**
			 * {@InheritDoc}
			 */
			virtual unsigned int computeRequiredNumberOfWeights() override;


		public:

			// -+-+-  Constructors/Destructors  -+-+- //

			ESNEigen(std::vector<real_t>& weights, size_t nbInputs, size_t nbOutputs, size_t reservoirSize = 100,
					real_t densityOfConnections = 0.10, real_t alpha = 0.88, seed_t seed = 0L,
					real_t inputToReservoirWeightRange = 0.5,
					real_t inputToOutputWeightRange = 0.5,
					real_t reservoirToOutputWeightRange = 0.5,
					real_t outputToReservoirWeightRange = 0.5,
					real_t outputToOutputWeightRange = 0.5,
					bool allowInputToOutputDirectConnections = false,
					bool allowOutputSelfRecurrentConnections = false,
					bool allowInputToReservoirConnections = true,
					bool fixedInputToReservoirConnections = true,
					bool allowOutputToReservoirConnections =  false,
					bool addConstantInputBias = false,
					bool addSinInputBias = false,
					real_t sinBiasPeriod = 0.01,
					bool useSparseComputation = false
					);

			/** Deep Copy constructor */
			ESNEigen(ESNEigen const& other);

			virtual ~ESNEigen() { }


			// -+-+-  Accessors/Mutators  -+-+- //

			/**
			 * Accessor for the neurons values
			 */
			inline const Eigen::VectorXd& getNeuronsValues() { return _neuronsValues; }

			/**
			 * Accessor for densityOfConnections
			 */
			inline unsigned int getDensityOfConnections() const { return _densityOfConnections; }

			/**
			 * Accessor for alpha
			 */
			inline unsigned int getAlpha() const { return _alpha; }

			/**
			 * Accessor for seed
			 */
			inline seed_t getSeed() const { return _seed; }


			// -+-+-  Main Methods  -+-+- //

			/**
			 * {@InheritDoc}
			 */
			virtual ESNEigen* clone() const override { return new ESNEigen(*this); }

			/**
			 * {@InheritDoc}
			 */
			virtual std::string toString() const override { return ReservoirNeuralNetwork::toString(); }

			/**
			 * Perform several steps
			 */
			virtual void step(size_t nbOfSteps);

			/**
			 * {@InheritDoc}
			 */
			virtual void step() override;

			/**
			 * Return a string identifying this class
			 */
			inline static std::string getNNTypeName() { return "ESNEigen"; }

			virtual size_t getNbNeurons();

			/**
			 * @return the id of the first input neuron in the weights matrix and in neuronsValues vector
			 */
			inline size_t getStartInput() { return 0; }

			/**
			 * @return the id of the first reservoir neuron in the weights matrix and in neuronsValues vector
			 */
			inline size_t getStartReservoir() { return getStartInput() + _nbInputs + _nbAdditionalInputs; }

			/**
			 * @return the id of the first output neuron in the weights matrix and in neuronsValues vector
			 */
			inline size_t getStartOutput() { return getStartReservoir() + _reservoirSize; }

	};

}


#endif

