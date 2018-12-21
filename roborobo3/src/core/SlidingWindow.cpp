/*
 *  SlidingWindow.cpp
 */

#include "Utilities/SlidingWindow.h"

SlidingWindow::SlidingWindow( int size )
{
    _currentIndex = _size-1; // so that first pushed value is at index 0. _currentIndex always points to most recent value.
    _size = size;
    _sum = 0;
    _data.reserve(_size);
    for ( int i = 0 ; i != _size ; i++ )
        _data.push_back(0.0);
    _alwaysPositiveSum = false;
}

void SlidingWindow::setAlwaysPositiveSum(bool b)
{
    _alwaysPositiveSum = b; 
}

size_t SlidingWindow::getSize()
{
    return _size;
}

double SlidingWindow::getAverageValue()
{
    return getSum()/_size;
}

double SlidingWindow::getSum()
{
    return _sum;
}

void SlidingWindow::reset( double value )
{
    _sum = 0;
    _currentIndex = _size-1;;
    for ( int i = 0 ; i < _size ; i++ )
    {
        _data[i] = value;
        _sum += value;
    }
}

void SlidingWindow::reset( int value )
{
    reset( (double)value );
}

void SlidingWindow::push( double value )
{
    if ( _alwaysPositiveSum == true && value < 0 )
    {
        std::cout << "[ERROR] SlidingWindows does not allow pushing a negative (" << value << ") value (_alwaysPositiveSum=true). Exit.\n";
        exit (-1);
    }

    _currentIndex = ( _currentIndex + 1 ) % _size;
    _sum = _sum - _data[_currentIndex];
    if ( _alwaysPositiveSum == true && _sum < 0 )
    {
        //std::cout << "[DEBUG] SlidingWindows with _alwaysPositiveSum=true -- _sum is upped to zero (was: " << _sum << ").\n";
        _sum = 0;
    }
    _sum = _sum + value;
    _data[_currentIndex] = value;

    /*
    // todelete after february 1st 2019
     sum2 = 0;
    for ( int i = 0 ; i < _size ; i++ )
        sum2 = sum2 + _data[i];
    if ( sum2 != _sum )
    {
        std::cout << "[DEBUG] BUG DETECTED (POST)\n";
        std::cout << "\tsum  = " << _sum << "\n";
        std::cout << "\tsum2 = " << sum2 << "\n";
        display();
    }
     */
}

void SlidingWindow::push( int value )
{
    push((double)value);
}

void SlidingWindow::displayInfo()
{
    std::cout << "[SLIDING WINDOW]\n";
    std::cout << "\tindex  = " << _currentIndex << "\n";
    std::cout << "\tsize   = " << _size << "\n";
    std::cout << "\tsum    = " << _sum << "\n";
    std::cout << "\tdata (ending at index)\n\t";
    
    for ( int i = 1 ; i != _size+1 ; i++ )
        std::cout << _data[ ( _currentIndex + i ) % _size ] << " ";
    std::cout << "\n";
}

double SlidingWindow::getPrev( int t )
{
    if ( t <= 0 and t >= -(_size-1) ) // t in [-size+1,0]
    {
        //std::cout << "[DEBUG] _currentIndex=" << _currentIndex << " - reqIndex=" << ((_currentIndex - t + _size ) % _size)  << "\n";
        return ( _data[ ( _currentIndex + t + _size ) % _size ] );
    }
    else
    {
        std::cout << "[ERROR] SlidingWindow::getPrev(t) -- t out of bound (t=" << t << ", not in [" << -(_size-1) << ",0]). Exit. \n";
        exit (-1);
    }
}
