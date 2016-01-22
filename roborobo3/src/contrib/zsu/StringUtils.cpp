//
//  Rev:          $Id: StringUtils.cpp,v 1.8 2005/08/08 14:13:15 evert Exp $
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
                
//static const char* sccsid = "$Id: StringUtils.cpp,v 1.8 2005/08/08 14:13:15 evert Exp $";

#include <algorithm>
#include <ctime>
#include <cstdio>
#include <cctype>
#include <fstream>
#include <functional>
#include <locale>
#include <map>
                                 
#include <zsu/func.h>  
//#include <zsu/strip.h>

#include <zsu/StringUtils.h>
//---------------------------------------------------------------------------

using namespace std;

//---------------------------------------------------------------------------
string zsu::escape(const string& str)
{
  const static string escapedChars("\t\n\r\\\"");

  // count number of characters that need to be escaped
  string::size_type extension = 0;
  for (string::const_iterator i = str.begin(); i != str.end(); ++i)
  {
    if (escapedChars.find(*i) != string::npos)
    {
      ++extension;
    }
  }

  // allocate sufficient memory
  string result;
  result.reserve(str.size() + extension);

  // copy string while performing substitutions
  for (string::const_iterator i = str.begin(); i != str.end(); ++i)
  {
    switch (*i)
    {
      case '\t':
      {
        result += "\\t";
        break;
      }
      case '\n':
      {
        result += "\\n";
        break;
      }
      case '\r':
      {
        result += "\\r";
        break;
      }
      case '\\':
      {
        result += "\\\\";
        break;
      }
      case '\"':
      {
        result += "\\\"";
        break;
      }
      default:
      {
        result += *i;
        // break;
      }
    }
  }

  return result;
}

//---------------------------------------------------------------------------
string zsu::unEscape(const string& str)
{
  string result("");

  for (string::const_iterator i = str.begin(); i != str.end(); ++i)
  {
    if (*i == '\\')    // we have an escapee
    {
      if (++i == str.end())
      {
        break;
      }
      //else

	    switch(*i)
      {
        case 't':
        {
          result += '\t';
          break;
        }

        case 'n':
        {
          result += '\n';
          break;
        }

	      case 'r':
        {
          result += '\r';
          break;
        }

        case '"':
        {
          result += '\"';
          break;
        }

        case '\\':
        {
          result += '\\';
          break;
        }

        default:
        { // ignore the backslash
          result += *i;
          //break;
        }
      }
    }
    else
    {   
      // we have an ordinary character
      result += *i;
    }
  }

  return result;
}

//---------------------------------------------------------------------------
string zsu::quote(const string& str)
{
    string result;
    result.reserve(str.size() + 2);
    result = "\"";
    result.append(str.begin(), str.end());
    result.append("\"");

    return result;
}

//---------------------------------------------------------------------------
string zsu::unQuote(const string& str)
{
    string::const_iterator from = str.begin();
    if (*from == '"')
    {
        from++;
    }

    string::const_iterator to = str.end();
    to--;
    if (*to != '"')
    {
        to++;
    }

    return string(from, to);
}

//---------------------------------------------------------------------------
bool zsu::readQuoted(string& str, istream& is)
{
    if (is.peek() != '"')
    {
        return false;
    }
    // else

    is.ignore();    // skip quote

    getline(is, str, '\"');    // "

    if (str.empty())
    {
        return true;
    }
    // else

    while (str[str.size() - 1] == '\\' && is)
    {
        string nextPart;
        getline(is, nextPart, '\"');  // "

	str += '\"'; // "
        str += nextPart;
    }

    return true;
}

//---------------------------------------------------------------------------
void zsu::readString(string& str, istream& is, char delim)
{
  string escaped;

  if (readQuoted(escaped, is))
  {
    if (delim != '\0')
    {
      string ignore;  // read but ignore 'till delimiter
      getline(is, ignore, delim);
    }
  }
  else
  {
    if (delim == '\0')
    {
      is >> escaped;
    }
    else
    {
      getline(is, escaped, delim);
    }
  }

  str = zsu::unEscape(escaped);
}
//---------------------------------------------------------------------------
bool zsu::checkKey(const string& key, istream& is)
{
    ios::iostate isState = is.rdstate();
    long isPos = is.tellg();

    string token;
    getline(is, token, '=');

    string::size_type endWhitespace = token.find_first_not_of(" \t\n\r");
    string::size_type startWhitespace = token.find_last_not_of(" \t\n\r");
    if (startWhitespace == string::npos)   // only whitespace found
    {
        return false;
    }
    // else

    string strippedToken(token, endWhitespace, (startWhitespace + 1) -
                                                                 endWhitespace);

    if (key.compare(strippedToken) == 0)
    {
      return true;
    }
    else
    {
      is.setstate(isState);
      is.seekg(isPos);
      return false;
    }
}

//---------------------------------------------------------------------------
bool zsu::readEntry(const string& key, string& value, istream& is)
{
  if (checkKey(key, is))
  {
    is >> ws;

    readString(value, is);

    return true;
  }
  // else

  return false;
}

//---------------------------------------------------------------------------
bool zsu::readSection(const string& key, string& value, istream& is)
{
  if(is.get() != '[' ) return false;

  if( !readEntry( key, value, is ) ) return false;

  is >> ws;

  if(is.get() != ']') return false;

  return true;
}

//---------------------------------------------------------------------------
bool zsu::readEntry(const string& key, vector<string>& values, istream& is)
{
    if (checkKey(key, is))
    {
        is >> ws;

        if (is.get() != '{')
        {
            return false;
        }
        // else

        is >> ws;

        string item;
        while (is.peek() != '}' && is)
        {
            if (!readQuoted(item, is))
            {
                is >> item;

                if (item.size() != 0)
                {
                    string::iterator lastChar = item.end();
                    lastChar--;

                    if (*lastChar == '}')    // oops, read delimiter
                    {
                        item.erase(lastChar);
                        is.putback('}');
                    }
                }
            }

            values.push_back(zsu::unEscape(item));

            is >> ws;
        }

        is.ignore();              // skip closing delimiter

        return true;
    }
    // else

    return false;
}

//---------------------------------------------------------------------------
bool zsu::readEntry(const string& key, double& value, istream& is)
{
    if (checkKey(key, is))
    {
        is >> ws >> value;

        return true;
    }
    // else

    return false;
}

//---------------------------------------------------------------------------
bool zsu::readEntry(const string& key, int& value, istream& is)
{
    if (checkKey(key, is))
    {
        is >> ws >> value;

        return true;
    }
    // else

    return false;
}

//---------------------------------------------------------------------------
bool zsu::readEntry(const string& key, vector<double>& values, istream& is)
{
  if (checkKey(key, is))
  {
    is >> ws;

    if (is.get() != '{')
    {
      return false;
    }
    // else

    is >> ws;

    double item;
    while (is.peek() != '}' && is)
    {
      is >> item;

      if (is.fail())
      {
        break;
      }

      values.push_back(item);

      is >> ws;
    }

    return is.get() == '}';              // skip & check closing delimiter
  }
  // else

  return false;
}

//---------------------------------------------------------------------------
bool zsu::readEntry(const string& key, vector<int>& values, istream& is)
{
  if (checkKey(key, is))
  {
    is >> ws;

    if (is.get() != '{')
    {
      return false;
    }
    // else

    is >> ws;

    int item;
    while (is.peek() != '}' && is)
    {
      is >> item;

      if (is.fail())
      {
        break;
      }

      values.push_back(item);

      is >> ws;
    }

    return is.get() == '}';              // skip & check closing delimiter
  }
  // else

  return false;
}

//---------------------------------------------------------------------------
bool zsu::startList(const string& key, istream& is)
{
    if (checkKey(key, is))
    {
        is >> ws;               // skip whitespace
        return is.get() == '{';
    }
    // else

    return false;
}

//---------------------------------------------------------------------------
void zsu::writeQuoted(const string& str, ostream& os)
{
  os << zsu::quote(str);
}

//---------------------------------------------------------------------------
void zsu::writeKey(const string& key, ostream& os)
{
  os << key << " = ";
}

//---------------------------------------------------------------------------
void zsu::writeEntry(const string& key, const string& value, ostream& os)
{
  zsu::writeKey(key, os);

  zsu::writeQuoted(zsu::escape(value), os);

  os << endl;
}

//---------------------------------------------------------------------------
void zsu::writeEntry(const string& key, const vector<string>& values, ostream& os)
{
  zsu::writeKey(key, os);

  os << "{ ";

  for (vector<string>::const_iterator i = values.begin(); i != values.end(); ++i)
  {
    zsu::writeQuoted(zsu::escape(*i), os);
    os << ' ';
  }

  os << "}\n";
}

//---------------------------------------------------------------------------
void zsu::writeEntry(const string& key, double value, ostream& os)
{
  zsu::writeKey(key, os);

  os << value << endl;
}

//---------------------------------------------------------------------------
void zsu::writeEntry(const string& key, int value, ostream& os)
{
  zsu::writeKey(key, os);

  os << value << endl;
}

//---------------------------------------------------------------------------
void zsu::writeEntry(const string& key, const vector<double>& values, ostream& os)
{
  zsu::writeKey(key, os);

  os << "{ ";

  for (vector<double>::const_iterator i = values.begin(); i != values.end(); ++i)
  {
    os << *i << ' ';
  }

  os << "}\n";
}

//---------------------------------------------------------------------------
void zsu::writeEntry(const string& key, const vector<int>& values, ostream& os)
{
  zsu::writeKey(key, os);

  os << "{ ";

  for (vector<int>::const_iterator i = values.begin(); i != values.end(); ++i)
  {
    os << *i << ' ';
  }

  os << "}\n";
}

//---------------------------------------------------------------------------
bool zsu::readList(vector<string>& values, istream& is)
{
  is >> ws;

  if (is.get() != '{')
  {
    return false;
  }
  // else

  is >> ws;

  string item;
  while (is.peek() != '}' && is)
  {
    if (!readQuoted(item, is))
    {
      is >> item;

      if (item.size() != 0)
      {
        string::iterator lastChar = item.end();
        lastChar--;

        if (*lastChar == '}')    // oops, read delimiter
        {
          item.erase(lastChar);
          is.putback('}');
        }
      }
    }

    values.push_back(zsu::unEscape(item));

    is >> ws;
  }

  is.ignore();              // skip closing delimiter

  return true;
}
//---------------------------------------------------------------------------
// See Stroustrup 3rd edition page 591
int zsu::compareCaseInsensitive(string a, string b)
{
  string::const_iterator p = a.begin();
  string::const_iterator q = b.begin();

  while (p != a.end() && q != b.end()) {
    if (toupper(*p) != toupper(*q)) return (toupper(*p) < toupper(*q)) ? -1 : 1;
    ++p;
    ++q;
  }

  return (a.size() == b.size()) ? 0 :
    (a.size() < b.size()) ? -1 : 1;
}

//---------------------------------------------------------------------------
string zsu::toLower(string text)
{
  // TODO: there must be a smarter way to do this...

  string result;
  result = text;

  string::iterator q = result.begin();
  string::const_iterator p = text.begin();
  while (p != text.end()) {
    *q = tolower(*p);

    ++p;
    ++q;
  }

  return result;
}

//---------------------------------------------------------------------------
string zsu::toUpper(string text)
{
  // TODO: there must be a smarter way to do this...

  string result;
  result = text;

  string::iterator q = result.begin();
  string::const_iterator p = text.begin();
  while (p != text.end()) {
    *q = toupper(*p);

    ++p;
    ++q;
  }

  return result;
}

//---------------------------------------------------------------------------
string zsu::toProperCase(string text)
{
  // TODO: there must be a smarter way to do this...

  string result;
  result = text;

  string::iterator q = result.begin();
  string::const_iterator p = text.begin();
  while (p != text.end()) {
    if (p == text.begin() || !isalpha(*(p-1)))
    {
      *q = toupper(*p);
    }
    else if (isalpha(*p))
    {
      *q = tolower(*p);
    }

    ++p;
    ++q;
  }

  return result;
}          
//---------------------------------------------------------------------------
string zsu::maskedSubstr(string str, string mask, char passThrough)
{
  string retval;

  string::iterator c, m;
  for (c = str.begin(), m = mask.begin();
       c != str.end() && m != mask.end();
       ++c, ++m)
  {
    if (*m == passThrough)
    {
      retval += *c;
    }
  }

  return retval;
}
//---------------------------------------------------------------------------
string zsu::removeMatchingQuotes(string str)
{
  typedef string::iterator iterator;

  string quotes("'\"");
  iterator opening(find_first_of(str.begin(), str.end(), quotes.begin(), quotes.end()));
  string retval(str.begin(), opening);

  while (opening != str.end())
  {
    string lastQuote(1, *opening);
    iterator closing( find_first_of(opening+1, str.end(), lastQuote.begin(), lastQuote.end()) );

    if (closing == str.end()) // No matching quote found
    {
      iterator nextOpening( find_first_of(opening+1, str.end(), quotes.begin(), quotes.end()) );
      retval += string(opening, nextOpening);
      opening = nextOpening;
    }
    else
    {
      retval += string(opening+1, closing);
      opening = find_first_of(closing+1, str.end(), quotes.begin(), quotes.end());
      retval += string(closing+1, opening);
    }
  }

  return retval;
}
//---------------------------------------------------------------------------
string zsu::stripLeft(string str, char padding)
{
  size_t pos = str.find_first_not_of( string(1, padding) );
  if (pos != string::npos)
    return string(str, pos);
  // else
  return str;
}
//---------------------------------------------------------------------------
string zsu::stripRight(string str, char padding)
{
  size_t pos = str.find_last_not_of( string(1, padding) );
  
  if (pos != string::npos)
    return string(str.begin(), str.begin() + pos + 1);
  // else
  return str;
}
//---------------------------------------------------------------------------
string zsu::stripAfter(string str, char padding)
{
  size_t pos = str.find_first_of( string(1, padding) );
  
  if (pos != string::npos)
    return string(str.begin(), str.begin() + pos - 1);
  // else
  return str;
}
//---------------------------------------------------------------------------
string zsu::strip(string str, char padding)
{
  return stripRight(stripLeft(str, padding), padding);
//return stripRight(stripLeft(str)); //tmp
}
//---------------------------------------------------------------------------

static string __spaces("=: \t\n\r\v\f");

string zsu::stripRight(string str)
{
  //size_t pos = str.find_first_not_of( __spaces );
  size_t pos = str.find_last_not_of( __spaces );

  if (pos != string::npos)
    return string(str.begin(), str.begin() + pos + 1);
  // else
  return str;

//  return strip_right_if(str, zsu::unary_function_binder<int, int>(std::isspace));
}
//---------------------------------------------------------------------------
string zsu::stripLeft(string str)
{
  size_t pos = str.find_first_not_of( __spaces );
  
  if (pos != string::npos)
    return string(str, pos);
	//return string(str.begin()+pos, str.end());
  // else
  return str;
//  return strip_left_if(str, unary_function_binder<int, int>(std::isspace));
}
//---------------------------------------------------------------------------
string zsu::strip(string str)
{
  return stripRight(stripLeft(str));
}
//---------------------------------------------------------------------------

