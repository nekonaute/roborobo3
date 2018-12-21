/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */

#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

#include "Utilities/Misc.h"

class SlidingWindow {

    private:
    
        std::vector<double> _data;
        int _currentIndex; 
        double _sum;
        int _size;
    
        bool _alwaysPositiveSum;
    
    public:
    
        SlidingWindow( int length );
    
        size_t getSize();
    
        double getAverageValue();
        double getSum();
    
        void setAlwaysPositiveSum( bool b ); // used to avoid problem with multiple plus/minys operations with  double (accuracy is not guaranteed) -- if true, _sum alsways >= 0, and pushed values must always be positive.
    
        void push( double value );
        void push( int value );
    
        void displayInfo();
    
        void reset( double value );
        void reset( int value );
    
        double getPrev(int t); // return value at -t (from 0 to -(size-1))
};

#endif
