/*
 *  Geometry.h
 *  roborobo
 *
 *  Created by Nicolas on 03/12/2013.
 *
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector> 
#include <math.h> 

#include <string>
#include <sstream>
#include <iostream>

#include <sys/time.h>

// A useful struct/class for storing 2D real coordinates.
class Point2d
{
	public:
		Point2d()
		{y=0;x=0;};
		Point2d(double inX, double inY){
			x = inX;
			y = inY;
		};
		double x;
		double y;
};

// return euclidian distance btw two 2D points
inline double getEuclidianDistance(double x1, double y1, double x2, double y2)
{
	return sqrt( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) );
}

// return the square of the euclidian distance btw two 2D points.
// hint: comparing squared values is faster than using squareroot...
inline double getSquaredEuclidianDistance(double x1, double y1, double x2, double y2)
{
	return ( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) );
}

// return euclidian distance btw two 2D points of type Point2d
inline double getEuclidianDistance(Point2d p1, Point2d p2)
{
	return sqrt( pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2));
}

double getSquaredEuclidianDistance(double x1, double y1, double x2, double y2);


// return distance and orientation of targetPt wrt. refPt+orientation (takes into account refPt orientation)
// returns an angle result in DEGREE, in [-180°,+180°[
// e.g. getAngleToTarget(robot.coord, robot.orientation, targert.coord) => angle to target
inline double getAngleToTarget( Point2d refPt , double orientation, Point2d targetPt )
{
    double angle;
    angle = atan2(targetPt.y-refPt.y,targetPt.x-refPt.x);
    if ( angle < 0 )
        angle = angle + 2*M_PI;
    angle = angle * 180.0 / M_PI;
    angle = angle - orientation;
    if ( angle >= 180 )
        angle = angle - 360;
    
    return angle;
}

inline double getAngleToTarget( double Ax, double Ay, double orientation, double Bx, double By)
{
    Point2d refPt = Point2d(Ax,Ay);
    Point2d targetPt = Point2d(Bx,By);
    return getAngleToTarget(refPt,orientation,targetPt);
}


#endif // GEOMETRY_H

