/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <neuralnetworks/MLP.h>
#include <string>
#include <vector>


namespace Neural {

	/**
	 * A basic Perceptron
	 * @author Leo Cazenille <leo.cazenille@upmc.fr>
	 */
	class Perceptron : public MLP {

		protected:

			/**
			 * {@InheritDoc}
			 */
			virtual unsigned int computeRequiredNumberOfWeights();


		public:

			// -+-+-  Constructors/Destructors  -+-+- //

			Perceptron(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs);
			/** Deep Copy constructor */
			Perceptron(Perceptron const& other);
			virtual ~Perceptron();


			// -+-+-  Main Methods  -+-+- //

			/**
			 * {@InheritDoc}
			 */
			virtual Perceptron* clone() const;

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

