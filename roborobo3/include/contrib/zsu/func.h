//
//  Rev:          $Id: func.h,v 1.1 2005/03/02 12:37:21 evert Exp $
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

#ifndef FUNC_H
#define FUNC_H

//#include <zsu/config.h>

#include <algorithm>

namespace zsu
{

//---------------------------------------------------------------------------
/**
  Template class to easily create \c std::unary_function wrappers for arbitrary
  unary functions.

  For example:
  \code
    #include <zsu/func.h>
                                 
    zsu::unary_function_binder<int, int> pred(std::isspace);
    std::string str("foo bar");
    std::string::iterator i = std::find_if(str.begin(), str.end(), pred);
    // i now points to the 4th character in str; i.e. the first space
  \endcode

 */
template <class _Result, class _Arg>
class unary_function_binder: public std::unary_function<_Result, _Arg>
{
  public:
    typedef _Result (*_Function)(_Arg);

    unary_function_binder(_Function __f) {__function = __f;}

    _Result operator() (_Arg __arg) {  return __function(__arg); }

  private:
    _Function __function;
};

//---------------------------------------------------------------------------

}
#endif

