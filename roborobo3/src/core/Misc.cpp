/*
 *  Misc.cpp
 *  roborobo
 *
 *  Created by Nicolas on 24/01/09.
 *
 */

#include "Utilities/Misc.h"
#include "RoboroboMain/common.h"

// useful functions. 
// see header file for description and credits/sources.


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
			//x1 = ((double)rand()/(1.0+(double)RAND_MAX)); //??? check and delete
			x1 = 2.0 * ranf() - 1.0;
			x2 = 2.0 * ranf() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( ( w >= 1.0 ) || ( w == 0.0 ) );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}


double computeModulo( double a , double b )
{
	int result = static_cast<int>( a / b );
	return a - static_cast<double>( result ) * b;
}


std::string getCurrentTimeAsReadableString()
{
	// --- get current time information
	
	time_t now = time(0);
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
