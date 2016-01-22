/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#ifndef RESERVOIRNEURALNETWORK_H
#define RESERVOIRNEURALNETWORK_H 

#include <neuralnetworks/NeuralNetwork.h>


namespace Neural {

	/**
	 * Definition of a reservoir neural network
	 * @author Leo Cazenille <leo.cazenille@upmc.fr>
	 */
	class ReservoirNeuralNetwork : public NeuralNetwork {

		protected:

			/** Size of the reservoir, ie of neurons in the reservoir */
			unsigned int _reservoirSize;

			/* Ranges of weights */
			double _inputToReservoirWeightRange;
			double _inputToOutputWeightRange;
			double _reservoirToOutputWeightRange;
			double _outputToReservoirWeightRange;
			double _outputToOutputWeightRange;

			/* Flags to define which connections are allowed */
			bool _allowInputToOutputDirectConnections;
			bool _allowOutputSelfRecurrentConnections;
			bool _allowInputToReservoirConnections;
			bool _allowOutputToReservoirConnections;

			/**
			 * @return the number of neurons
			 */
			size_t getNbNeurons();


		public :

			// -+-+-  Constructors/Destructors  -+-+- //

			virtual ~ReservoirNeuralNetwork();

			ReservoirNeuralNetwork(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, unsigned int reservoirSize,
					double inputToReservoirWeightRange,
					double inputToOutputWeightRange,
					double reservoirToOutputWeightRange,
					double outputToReservoirWeightRange,
					double outputToOutputWeightRange,
					bool allowInputToOutputDirectConnections,
					bool allowOutputSelfRecurrentConnections,
					bool allowInputToReservoirConnections,
					bool allowOutputToReservoirConnections);

			ReservoirNeuralNetwork(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, unsigned int reservoirSize);

			/** Deep Copy constructor */
			ReservoirNeuralNetwork(ReservoirNeuralNetwork const& other);


			// -+-+-  Accessors/Mutators  -+-+- //

			/**
			 * Accessor for reservoirSize
			 */
			unsigned int getReservoirSize() const;

			/**
			 * Accessor for allowInputToOutputDirectConnections
			 */
			bool areInputToOutputDirectConnectionsAllowed() const;

			/**
			 * Accessor for allowOutputSelfRecurrentConnections
			 */
			bool areOutputSelfRecurrentConnectionsAllowed() const;

			/**
			 * Accessor for allowInputToReservoirConnections
			 */
			bool areInputToReservoirConnectionsAllowed() const;

			/**
			 * Accessor for allowOutputToReservoirConnections
			 */
			bool areOutputToReservoirConnectionsAllowed() const;



			// -+-+-  Main Methods  -+-+- //

			/**
			 * {@InheritDoc}
			 */
			virtual ReservoirNeuralNetwork* clone() const = 0;

			/**
			 * {@InheritDoc}
			 */
			virtual std::string toString() const;


	};


}

#endif

