/**
 * @file
 * @author Leo Cazenille <leo.cazenille@upmc.fr>
 *
 *
 */

#ifndef ELMAN_H
#define ELMAN_H 

#include <neuralnetworks/MLP.h>
#include <string>
#include <vector>


namespace Neural {

	/**
	 * Multi-layers version of Elman SRN
	 * @author Leo Cazenille <leo.cazenille@upmc.fr>
	 */
	class Elman : public MLP {

		protected:

			/** Values of the hidden layers at the last step */
			std::vector< std::vector<double> > _lastOutputs;

			/**
			 * Initialize lastOutputs
			 */
			void initLastOutputs();

			/**
			 * {@InheritDoc}
			 */
			virtual unsigned int computeRequiredNumberOfWeights();


		public:

			// -+-+-  Constructors/Destructors  -+-+- //

			Elman(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			Elman(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, std::vector<unsigned int>& nbNeuronsPerLayer, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			/** Deep Copy constructor */
			Elman(Elman const& other);
			virtual ~Elman();


			// -+-+-  Accessors/Mutators  -+-+- //

			/** Accessor for lastOutputs */
			std::vector< std::vector<double> > const& getLastOutputs() const;


			// -+-+-  Main Methods  -+-+- //

			/**
			 * {@InheritDoc}
			 */
			virtual Elman* clone() const;

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

