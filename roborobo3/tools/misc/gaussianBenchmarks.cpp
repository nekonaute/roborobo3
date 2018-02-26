/*
 * Testing random number generator speed
 * boost's MT vs old-style, innacurate, rand.
 *
 * how to compile under macos, command-line: 
 *		clang++ -std=c++11 -stdlib=libc++ -Weverything filename.cpp
 *
 */

#include <iostream>
#include <random>
#include <chrono>

double getRand();
double getGaussianRand(double m, double s);

std::random_device rnd;
std::mt19937 engine(rnd());
std::uniform_real_distribution<double> disRandom(0.0, 1.0);
std::uniform_int_distribution<> disRandint(0, UINT32_MAX);
std::normal_distribution<> disNormal(0,1);

//#define random() disRandom(engine) // uniform in [0,1), return double
inline double random01() { return disRandom(engine); }
#define randint() disRandint(engine) // uniform in [0,max), returns int
#define randgaussian() disNormal(engine) // normal distribution mean=0 and stddev=1 (use: mean+rand*stddev)

// gaussian random adapted from: http://www.design.caltech.edu/erik/Misc/Gaussian.html
double getGaussianRand(double m, double s)	/* normal random variate generator */
{				        /* mean m, standard deviation s */
	double x1, x2, w, y1;
	static double y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			//x1 = ((double)randint()/(1.0+(double)RAND_MAX)); //??? check and delete
			x1 = 2.0 * random01() - 1.0;
			x2 = 2.0 * random01() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( ( w >= 1.0 ) || ( w == 0.0 ) );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}



int main () //int argc, char *argv[])
{

	int it = 100000000;
    double value = 0, mean = 0;
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    std::cout << "BOOST/C++11 GAUSSIAN\n";
    for ( int k = 0 ; k < it ; k++ )
    {
        value = randgaussian();
        mean += value;
        //std::cout << value << std::endl;
    }
    std::cout << "Mean: " << mean/it << std::endl;
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t2-t1;
    std::cout << "[1] " << diff.count() << "ms.\n";

    t1 = std::chrono::high_resolution_clock::now();
    mean = 0;
    std::cout << "NATIVE GAUSSIAN\n";
    for ( int k = 0 ; k < it ; k++ )
    {
        value = getGaussianRand(0,1);
        mean += value;
        //std::cout << value << std::endl;
    }
    std::cout << "Mean: " << mean/it << std::endl;
    t2 = std::chrono::high_resolution_clock::now();
    diff = t2-t1;
    std::cout << "[2] " << diff.count() << "ms\n";
}
