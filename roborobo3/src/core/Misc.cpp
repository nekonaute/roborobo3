/*
 *  Misc.cpp
 *  roborobo
 *
 *  Created by Nicolas on 24/01/09.
 *
 *  A set of useful functions.
 *  see header file for description and credits/sources.
 */

#include "Utilities/Misc.h"
#include "RoboroboMain/common.h"
#include <string>
#include <random>

// random generator functions, header declaration in common.h (general scope)
std::random_device rnd;
std::minstd_rand randint;
std::mt19937 engine(rnd());
std::uniform_real_distribution<double> disRandom(0.0, 1.0);
std::normal_distribution<> disNormal(0,1);

double getBoundedGaussianMutatedValue(double __value, float sigma, double __minValue, double __maxValue )
{
    double retValue = __value + randgaussian()*sigma;
    
    // bouncing upper/lower bounds
    if ( __value < __minValue )
    {
        double range = __maxValue - __minValue;
        double overflow = - ( (double)__value - __minValue );
        overflow = overflow - 2*range * (int)( overflow / (2*range) );
        if ( overflow < range )
            __value = __minValue + overflow;
        else // overflow btw range and range*2
            __value = __minValue + range - (overflow-range);
    }
    else if ( __value > __maxValue )
    {
        double range = __maxValue - __minValue;
        double overflow = (double)__value - __maxValue;
        overflow = overflow - 2*range * (int)( overflow / (2*range) );
        if ( overflow < range )
            __value = __maxValue - overflow;
        else // overflow btw range and range*2
            __value = __maxValue - range + (overflow-range);
    }
    
    return retValue;
}

double getBalance( double a, double b )
{
    // x vs x => 1.0
    // x vs 0 => 0.0
    // 0 vs 0 => -1.0 (divbyzero protection)
    // f(a,b) = 1.0 - ( ( max(a,b) / ( a+b ) - 0.5 ) * 2.0 ) = 2. - 2. * max(a,b) / (a+b)
    // constraints: a>=0, b>=0
    
    if ( a+b == 0 )
        return ( -1. );
    else
        return ( 2. - 2. * std::max( float(a) , float(b) ) / float( a + b ) );
}

double getBalance( int a , int b )
{
    return getBalance( (double)a, (double)b );
}

std::string getpidAsReadableString()
{
    return boost::lexical_cast<std::string>((long)::getpid());
}

std::string getCurrentTimeAsReadableString()
{
	// --- get current time information
	
	time_t now = time(NULL);
	char timestamp[19] = "";
	//strftime (timestamp, 19,"%Y%m%d-%Hh%Mm%Ss", localtime(&now));
	strftime (timestamp, 19,"%Y%m%d-%Hh%Mm%Ss", localtime(&now));
	std::string s = timestamp;
	
	// --- get milliseconds resolution (note: could be used to replace code block above - left for tutorial)
	
	struct timeval now2;
    int mtime;
    gettimeofday(&now2, NULL);
	
	mtime = now2.tv_usec;
	if ( mtime < 100000 )
	{
		s+="0";
		if ( mtime < 10000 )
			s+="0";
		if ( mtime < 1000 )
			s+="0";
		if ( mtime < 100 )
			s+="0";
		if ( mtime < 10 )
			s+="0";
	}	
	s += convertToString(mtime) + "us"; // microseconds

	return s;

}

std::string convertToString( int __value )			
{
	std::string s;
	std::stringstream sOutTmp;
	sOutTmp << __value;
	s = sOutTmp.str();
	
	return s;
}

void sleep(int milliseconds)
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}
