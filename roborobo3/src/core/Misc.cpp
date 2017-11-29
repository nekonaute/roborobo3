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
std::mt19937 engine(rnd());
std::uniform_real_distribution<double> disRandom(0.0, 1.0);
std::uniform_int_distribution<> disRandint(0, UINT32_MAX);
std::normal_distribution<> disNormal(0,1);

double computeModulo( double a , double b )
{
	int result = static_cast<int>( a / b );
	return a - static_cast<double>( result ) * b;
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
