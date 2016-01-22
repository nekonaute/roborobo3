/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H 


#include <vector>
#include <iostream>
#include <neuralnetworks/NeuralNetworkException.h>


namespace Neural {

	/**
	 * A simple interface definition of a neural network
	 * @author Leo Cazenille <leo.cazenille@upmc.fr>
	 */
	class NeuralNetwork {

		protected:

			/** A small message to describe this NN */
			std::string _description;

			/** Values of the weights */
			std::vector<double> _weights;

			/** Number of input neurons */
			unsigned int _nbInputs;

			/** Number of output neurons */
			unsigned int _nbOutputs;

			/** Values of the inputs */
			std::vector<double> _inputs;

			/** Values of the outputs */
			std::vector<double> _outputs;

			/** Required Number of weights */
			unsigned int _requiredNumberOfWeights;

			/** Says if the inputs have changed */
			bool _haveInputsChanged;

			/** Says if the outputs have changed */
			bool _haveOutputsChanged;

			/** Says if the weights have changed */
			bool _haveWeightsChanged;

			/**
			 * Return the required number of weight of a neural network
			 */
			virtual unsigned int computeRequiredNumberOfWeights() = 0;


		public:

			// -+-+-  Constructors/Destructors  -+-+- //

			virtual ~NeuralNetwork();
			NeuralNetwork(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs);

            /** Deep Copy constructor */
			NeuralNetwork(NeuralNetwork const& other);


			// -+-+-  Accessors/Mutators  -+-+- //

			/**
			 * Mutator for description
			 */
			virtual void setDescription(std::string description);

			/**
			 * Accessor for description
			 */
			virtual std::string getDescription() const;

			/** 
			 * Accessor for weights
			 */
			virtual std::vector<double> const& getWeigths() const;

			/** 
			 * Mutator for weights
			 */
			virtual void setWeigths(std::vector<double>& weights);

			/**
			 * Accessor for the input values
			 */
			virtual const std::vector<double>& getInputs() const;

			/**
			 * Mutator for the input values
			 */
			virtual void setInputs(std::vector<double>& inputs);

			/**
			 * Accessor the output values
			 */
			virtual const std::vector<double>& readOut() const;

			/**
			 * Mutator the output values
			 */
			virtual void setOutputs(std::vector<double>& outputs);

			/**
			 * Accessor for inputs number
			 */
			unsigned int getNbInputs() const;

			/**
			 * Accessor for outputs number
			 */
			unsigned int getNbOutputs() const;

			/**
			 * Accessor for requiredNumberOfWeights
			 */
			unsigned int getRequiredNumberOfWeights();


			// -+-+-  Main Methods  -+-+- //

			/**
			 * Clone method
			 * @return a clone of this object
			 */
			virtual NeuralNetwork* clone() const = 0;

			/**
			 * Pretty printer
			 * @return pretty print of a description of the object
			 */
			virtual std::string toString() const;

			/**
			 * Print infos (just call the pretty printer)
			 */
			void displayInfo() const;

			/**
			 * Write log in a log file
			 */
			virtual void writeLog() const;

			/**
			 * Do all neural computations
			 */
			virtual void step() = 0;


			// -+-+-  Operators Overloads  -+-+- //

			/**
			 * Pretty printer friend
			 */
			friend std::ostream& operator<<(std::ostream& os, NeuralNetwork const& p);

	};

}


#endif

