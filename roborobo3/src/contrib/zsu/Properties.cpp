//
//  Rev:          $Id: Properties.cpp,v 1.4 2005/08/08 14:13:15 evert Exp $
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
            
//static const char* sccsid = "$Id: Properties.cpp,v 1.4 2005/08/08 14:13:15 evert Exp $";

#include <zsu/Properties.h>
#include <zsu/StringUtils.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;
using namespace zsu;

//---------------------------------------------------------------------------

// Note: no need for copy c'tor or assignment coding.

Properties::Properties()
{}

//---------------------------------------------------------------------------

// Alternative for just using "map" methods
void Properties::setProperty(string key, string value) {
  operator[] (key) = value;
}

//---------------------------------------------------------------------------

static const string keyValueSeparators        = "=: \t\r\n\f";
static const string strictKeyValueSeparators  = "=:";
static const string specialSaveChars          = "=: \t\r\n\f#!";
static const string commentChars              = "#!";
static const string whiteSpaceChars           = " \t\r\n\f";

//---------------------------------------------------------------------------

void Properties::load(istream& inStream)
{
  typedef string::iterator iterator;
  while (inStream)
  {
    string line;
    getline(inStream, line);
    line = stripLeft(line);

    if (line.size() > 0)
    {
      // skip comments
      if (commentChars.find(line[0]) != string::npos)
      {
        continue;
      }
      //else

      // glue together lines that end in slashes
      while (continueLine(line) && inStream)
      {
        string nextLine;
        getline(inStream, nextLine);

        // skip starting whitespace on new line
        line += stripLeft(nextLine);
      }

      iterator keyStart(line.begin());
      iterator keyEnd( find_first_of(line.begin(), line.end(), keyValueSeparators.begin(), keyValueSeparators.end()) );
      string key(stripRight(string(keyStart, keyEnd)));

  	  // skip one non whitespace key value separators if any
      iterator valStart(keyEnd);
      if (valStart != line.end() && strictKeyValueSeparators.find(*valStart) != string::npos)
      {
        ++valStart;
      }

      string value( stripRight(stripLeft(stripAfter(string(valStart, line.end()),'#'))) );

      // Convert then store key and value
      key = loadConvert(key);
      value = loadConvert(value);
      operator[] (key) = value;
    }
  }
}

//---------------------------------------------------------------------------

/// Helper func to read a string, whitespace delimited but possibly quoted
string Properties::readString(istream& is)
{
  static const char quote('\"');
  string retval;

  ws(is); // strip leading whitespace

  if (is.peek() == quote)
  {
    is.ignore(); // skip opening quote

    getline(is, retval, quote);

    is.ignore(); // skip closing quote
  }
  else
  {
    is >> retval;
  }

  return retval;
}
//---------------------------------------------------------------------------

/// Helper func to read 'name=value' from a whitespace-delimited sequence
pair<string, string> Properties::readPropertyEntry(istream& is)
{
  string first(readString(is));

  if (first.size() == 0)  // if we can't read anything from the stream, return an empty pair
  {
    return pair<string, string>();
  }
  // else

  // if there was no whitespace between the name, the '=' and the value, first
  // may now contain the whole 'name=value' string - check if this is the case
  // and, if so, split first and return the result
  string::size_type pos = first.find('=');
  if (pos != first.npos)
  {
    // must call readString on remainder: it may be quoted or escaped
    string second(first, pos+1);
    istringstream is2(second);
    return pair<string, string>(string(first, 0, pos), readString(is2));
  }
  // else

  ws(is);

  if (is.peek() != '=') // we must be at '=' now, otherwise it wasn't a pair
  {
    return pair<string, string>();
  }
  // else

  is.ignore();
  ws(is);

  // read the value
  string second(readString(is));

  return pair<string, string>(first, second);
}
//---------------------------------------------------------------------------

void Properties::load(string props)
{
  istringstream s(props);

  while (s)
  {
    pair<string, string> propertyEntry = readPropertyEntry(s);

    if (propertyEntry.first.size() != 0)
    {
      setProperty(propertyEntry.first, propertyEntry.second);
    }
  }
}
//---------------------------------------------------------------------------

bool Properties::continueLine (string& line)
{
  string::iterator i(line.end()-1);

  int slashCount = 0;
  while (i != line.begin() && *i == '\\')
  {
    --i;
    ++slashCount;
  }

  if (slashCount % 2 == 1)
  {
    line.erase(i, line.end());

    return true;
  }
  // else

  return false;
}

//---------------------------------------------------------------------------

string Properties::loadConvert (string thestring) {
  char aChar;
  int len = (int)thestring.size();
  string outBuffer; /* capacity should be len */

  for(int x=0; x<len; ) {
    aChar = thestring.at(x++);
    if (aChar == '\\') {
      aChar = thestring.at(x++);
      if (aChar == 't') aChar = '\t';
      else if (aChar == 'r') aChar = '\r';
      else if (aChar == 'n') aChar = '\n';
      else if (aChar == 'f') aChar = '\f';
      outBuffer.append(1, aChar);
    } else
      outBuffer.append(1, aChar);
  }
  return outBuffer;
}

//---------------------------------------------------------------------------

string Properties::saveConvert(string thestring, bool escapeSpace) {
  int len = (int)thestring.size();
  string outBuffer; /* capacity should be set to 2*len */

  for(int x=0; x<len; x++) {
    char aChar = thestring.at(x);
    switch(aChar) {
    case ' ':
      if (x == 0 || escapeSpace)
        outBuffer.append(1, '\\');

      outBuffer.append(1, ' ');
      break;
    case '\\':outBuffer.append(1, '\\').append(1, '\\');
      break;
    case '\t':outBuffer.append(1, '\\').append(1, 't');
      break;
    case '\n':outBuffer.append(1, '\\').append(1, 'n');
      break;
    case '\r':outBuffer.append(1, '\\').append(1, 'r');
      break;
    case '\f':outBuffer.append(1, '\\').append(1, 'f');
      break;
    default:
      if (specialSaveChars.find(aChar) != string::npos)
        outBuffer.append(1, '\\');
      outBuffer.append(1, aChar);
    }
  }
  return outBuffer;
}

//---------------------------------------------------------------------------

void Properties::store(ostream& out)
{
  store(out, "");
}

//---------------------------------------------------------------------------

void Properties::store(ostream& out, string header)
{
  if (!header.empty())
    out << "#" << header << endl;

  for (map<string,string>::iterator itr=begin();
       itr != end();
       ++itr) {
    string key = (*itr).first;
    string val = (*itr).second;
    key = saveConvert(key, true);

    /* No need to escape embedded and trailing spaces for value, hence
     * pass false to flag.
     */
    val = saveConvert(val, false);
    out << key << "=" << val << endl;
  }
  out.flush();
}       

//---------------------------------------------------------------------------


bool Properties::hasProperty(string key) {
  map<string,string>::iterator itr = find(key);
  return itr != end();
}       

//---------------------------------------------------------------------------

string Properties::getProperty(string key) {
  map<string,string>::iterator itr = find(key);
  return (itr == end()) ? string("") : operator[] (key);
}      

//---------------------------------------------------------------------------

string Properties::getProperty(string key, string defaultValue) {
  map<string,string>::iterator itr = find(key);
  return (itr == end()) ? defaultValue : operator[] (key);
}   

//---------------------------------------------------------------------------

vector<string> Properties::addCommandLineArgs(int argc, char* argv[])
{
  vector<string> unusedArgs;
  const string CMDLINEARG_FILETAG = "-f";

  int curArgIdx = 0;
  while(curArgIdx < argc) {
    string curArg(argv[curArgIdx]);

    if (curArgIdx < (argc-1) && curArg == CMDLINEARG_FILETAG)
    {
      ifstream propsFile(argv[curArgIdx+1]);
      load(propsFile);
      curArgIdx += 2;
    }
    else if (curArgIdx < (argc-1) && curArg.find("-") == 0)
    {
      setProperty(curArg.substr(1), string(argv[curArgIdx+1]));
      curArgIdx += 2;
    }
    else
    {
      unusedArgs.push_back(curArg);
      curArgIdx++;
    }
  }

  return unusedArgs;
}
//---------------------------------------------------------------------------

void Properties::addEnvironment(std::string property)
{
  const char* value = std::getenv(property.c_str());
  if (value)
  {
    setProperty(property, value);
  }
}
//---------------------------------------------------------------------------


