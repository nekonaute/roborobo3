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

#include <cstdlib> // rand and RAND_MAX

#include <sys/time.h>

#define swapInteger(a,b) {int tmp;tmp=a;a=b;b=tmp;}


#define ranf() \
  ((double)rand()/(1.0+(double)RAND_MAX)) // Uniform from interval [0,1) */


/* Credits:
	boxmuller.c           Implements the Polar form of the Box-Muller
                         Transformation

                      (c) Copyright 1994, Everett F. Carter Jr.
                          Permission is granted by the author to use
			  this software for any application provided this
			  copyright notice is preserved.
			  http://www.taygeta.com/random/gaussian.html
			  http://www.bearcave.com/misl/misl_tech/wavelets/hurst/random.html
*/
double getGaussianRand (double m, double s); // box-muller



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

double computeModulo(double a, double b);

// return current time in a string, with readable format - e.g. 20100526-10h12m08s
// check: http://www.cplusplus.com/reference/clibrary/ctime/strftime/
std::string getCurrentTimeAsReadableString();

// check: http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
std::string convertToString( int __value );

void sleep(int milliseconds);

#endif // MISC_H

