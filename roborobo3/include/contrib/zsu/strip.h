//
//  Rev:          $Id: strip.h,v 1.1 2005/03/02 12:37:21 evert Exp $
//
//  Author:       Evert Haasdijk
//  Contact:      evert@zukkespijkers.nl
//
//  copyright (c) 2003 - 2004 Zukke Spijkers BV
//
//  This file is part of the Zukke Spijkers Utilities Library.
//
//  The Zukke Spijkers Utilities Library is free software; you can redistribute
//  it and/or modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the License,
//  or (at your option) any later version.
//
//  The Zukke Spijkers Utilities Library is distributed in the hope that it
//  will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
//  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with the Zukke Spijkers Utilities Library; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA  02111-1307  USA
//
//---------------------------------------------------------------------------

#ifndef STRIP_H
#define STRIP_H

//#include <zsu/config.h>

#include <algorithm>

namespace zsu
{

//---------------------------------------------------------------------------

/**
  Removes leading elements that match \a __pred from \a __container.

  Returns a new \c _Container with the result
 */
template <class _Container, class _Predicate>
_Container  strip_left_if(_Container __container, _Predicate __pred)
{
  typename _Container::iterator Iter;
  Iter from(std::find_if(__container.begin(), __container.end(), std::not1(__pred)));
  return _Container(from, __container.end());
}
//---------------------------------------------------------------------------

/**
  Removes trailing elements that match \a __pred from \a __container.  

  Returns a new \c _Container with the result
 */
template <class _Container, class _Predicate, typename __Iter = _Container::reverse_iterator >
_Container  strip_right_if(_Container __container, _Predicate __pred)
{
  __Iter from(std::find_if(__container.rbegin(), __container.rend(), std::not1(__pred)));
  _Container retval(from, __container.rend());
  std::reverse(retval.begin(), retval.end());

  return retval;
}
//---------------------------------------------------------------------------  
/**
  Removes leading and trailing elements that match \a __pred from \a __container.

  Returns a new \c _Container with the result
 */
template <class _Container, class _Predicate>
_Container  strip_if(_Container __container, _Predicate __pred)
{
  return strip_right_if(strip_left_if(__container, __pred), __pred);
}        
//---------------------------------------------------------------------------

}
      
#endif  // STRIP_H
 
