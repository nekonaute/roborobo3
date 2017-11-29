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

// get random number in [0,1[ as double
double getRand()
{
    std::random_device rnd;
    std::mt19937 engine(rnd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(engine);
}

int main () //int argc, char *argv[])
{

    int maxIt = 10000000;
    
    double value = 0;
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    for ( int k = 0 ; k < maxIt ; k++ )
    {
        value += getRand();
        //std::cout << "getRand(): " << value << std::endl;
    }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t2-t1;
    std::cout << "[1] " << diff.count() << "ms. (val=" << value/maxIt << ")\n";
    
    t1 = std::chrono::high_resolution_clock::now();
    value = 0;
    std::random_device rnd;
    std::mt19937 engine(rnd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    for ( int k = 0 ; k < maxIt ; k++ )
    {
        value += dis(engine);
    }
    
    t2 = std::chrono::high_resolution_clock::now();
    diff = t2-t1;
    std::cout << "[2] " << diff.count() << "ms. (val=" << value/maxIt << ")\n";

	srand(0);
    t1 = std::chrono::high_resolution_clock::now();
    value = 0;
    for ( int k = 0 ; k < maxIt ; k++ )
    {
        value += ((double) rand() / (RAND_MAX));
    }
    
    t2 = std::chrono::high_resolution_clock::now();
    diff = t2-t1;
    std::cout << "[3] " << diff.count() << "ms. (val=" << value/maxIt << ")\n";

}
