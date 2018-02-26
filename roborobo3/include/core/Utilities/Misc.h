/*
 *  Misc.h
 *  roborobo
 *
 *  Created by Nicolas on 24/01/09.
 *
 */

#ifndef MISC_H
#define MISC_H

#include <vector> 
#include <math.h> 

#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib> // RAND_MAX
#include <random>
#include <chrono>
#include <sys/time.h>

#define swapInteger(a,b) {int tmp;tmp=a;a=b;b=tmp;}

// random number generators (defined in roborobo.cpp)
// Note that boost/c++11 random is slower (by one order of magnitude), but more precise, than old classic rand()

extern std::random_device rnd;
extern std::minstd_rand randint; // randint() returns an int with value drawn uniformly in [0,max)
extern std::mt19937 engine;
extern std::uniform_real_distribution<double> disRandom;
extern std::normal_distribution<> disNormal;

inline double random01() { return disRandom(engine); } // uniform in [0,1), return double
inline double randgaussian() { return disNormal(engine); } // normal distribution mean=0 and stddev=1 (use: mean+rand*stddev)


// Cast string to value.
//
// cite: "the third parameter of from_string() should be one of std::hex, std::dec or std::oct"
// usage:
//  convertFromString<int>(i, std::string("ff"), std::hex))
//  convertFromString<float>(f, std::string("123.456"), std::dec))
// return: success/failure as boolean value
// source: http://www.codeguru.com/forum/showthread.php?t=231054
template <class T>
bool convertFromString(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

// provide a value for the balance between the two input values.
// f(a,b) = 1.0 - ( ( max(a,b) / ( a+b ) - 0.5 ) * 2.0 ) = 2. - 2. * max(a,b) / (a+b)
double getBalance( double a, double b );
double getBalance( int a , int b );

double computeModulo(double a, double b);

// return current time in a string, with readable format - e.g. 20100526-10h12m08s
// check: http://www.cplusplus.com/reference/clibrary/ctime/strftime/
std::string getCurrentTimeAsReadableString();

// return PID as a string. Useful to desambiguate timestamps
std::string getpidAsReadableString();

// check: http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
std::string convertToString( int __value );

void sleep(int milliseconds);

#endif // MISC_H

