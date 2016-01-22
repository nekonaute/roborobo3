/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#ifndef MLP_H
#define MLP_H 

#include <neuralnetworks/LayeredNeuralNetwork.h>
#include <string>
#include <vector>


namespace Neural {

	/**
	 * A basic Multi-Layers Perceptron
	 * @author Leo Cazenille <leo.cazenille@upmc.fr>
	 */
	class MLP : public LayeredNeuralNetwork {

		protected:

			/**
			 * {@InheritDoc}
			 */
			virtual unsigned int computeRequiredNumberOfWeights();


		public:

			// -+-+-  Constructors/Destructors  -+-+- //

			MLP(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			MLP(unsigned int nbInputs, unsigned int nbOutputs, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			MLP(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, std::vector<unsigned int>& nbNeuronsPerHiddenLayer, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			/** Deep Copy constructor */
			MLP(MLP const& other);
			virtual ~MLP();


			// -+-+-  Main Methods  -+-+- //

			/**
			 * {@InheritDoc}
			 */
			virtual MLP* clone() const;

			/**
			 * {@InheritDoc}
			 */
			virtual std::string toString() const;

			/**
			 * {@InheritDoc}
			 */
			virtual void step();

			/**
			 * Return a string identifying this class
			 */
			static std::string getNNTypeName();

	};

}


#endif

