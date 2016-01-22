//
//  Rev:          $Id: StringUtils.h,v 1.7 2005/03/02 12:37:21 evert Exp $
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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H
 
//#include <zsu/config.h>

#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <typeinfo>
#include <iomanip>

//---------------------------------------------------------------------------
/// dump an exception to a stream
/**
Writes the classname and e.what()

  \param os reference to the stream to dump \a e on
  \param e const reference to the exception to dump
*/
inline std::ostream& operator<<(std::ostream& os, const std::exception& e)
{
  std::string type = typeid(e).name(); // get rid of "class " prefix of type id
  size_t p = type.find_last_of(" ");
  if (p != std::string::npos) type.erase(0, p+1);

  return os << type << ": " << e.what();
}

//---------------------------------------------------------------------------

namespace zsu
{

  /**@name String manipulation functions */
  //@{

  /** string escape function.

    Escapes special characters in a string using the 'C' convention.

      @param str const reference to string to be escaped
      @return an escaped version of str
      @see unEscape()
  */
  std::string  escape(const std::string& str);

  /** string un-escape function.

    Replaces special characters in a string according to the 'C' convention.
    Any backslash not followed by 't', 'n', 'r' or another backslash will
    simply be removed from the string.

      @param str const reference to string to be un-escaped
      @return an un-escaped version of str
      @see escape()
  */
  std::string  unEscape(const std::string& str);

  /** string quote function.

    Encloses a string in quotes (").

      @param str const reference to string to be quoted
      @return a quoted version of str
      @see unQuote(), readQuoted()
  */
  std::string  quote(const std::string& str);

  /** string un-quote function.

    Removes first and last character from a string *if they are quotes (")*.
    If they are not, no change is made.

      @param str const reference to string to be un-quoted
      @return an un-quoted version of str
      @see quote(), readQuoted()
  */
  std::string  unQuote(const std::string& str);

  //---------------------------------------------------------------------------
  //@}



  /**@name Streamed input */
  //@{
  //---------------------------------------------------------------------------

  /** read quoted string from a stream.

    Reads a quoted string from a stream. Quotes can be escaped; if so,
    they're read as other characters. The quotes are removed from the
    stream, but not stored.
    If the first character on the stream is not a quote, the function
    returns false immediately.

      @param str reference to string that will contain the string without the
      quotes
      @param is reference to the stream that will be read
      @return a bool indicating success
      @see escape(), unEscape()
  */
  bool  readQuoted(std::string& str, std::istream& is);

  /** read a string from a stream.

    Reads a string from a stream. The string may be quoted, in which case
    the enclosing quotes are removed. Also, the string is un-escaped.
    If a delimiter is specified, it is extracted from the stream. If the
    delimiter is NULL (the default), strings are whitespace-delimited.
    Thus, NULL itself is not a valid delimiter.

      @param str reference to string that will contain the string
      @param is reference to the stream that will be read
      @param delim optional delimiting character.
      @return none
      @see readQuoted(), unEscape()
  */
  void  readString(std::string& str, std::istream& is, char delim = 0);

  /** reads and checks a key= pair.

    Checks whether 'key=' is at the current position of the stream. Ignores
    whitespace, so ' key=', 'key =' and ' key =' all match. If the string
    matches, it is read so that the current position of the stream advances
    to the first character after '='.

      @param key const reference to string to match (without the '=')
      @param is reference to the stream that will be read
      @return true if a match is found, false otherwise
  */
  bool  checkKey(const std::string& key, std::istream& is);

  /** reads and checks a stream for #"key={"#.

    Checks whether #key={# is at the current position of the stream. Ignores
    whitespace, so # key={#, #key ={# and # key = {# all match. If
    the string matches, it is read so that the current position of the
    stream advances to the first character after #{#.

      @param key const reference to string to match (without the #={#)
      @param is reference to the stream that will be read
      @return true if a match is found, false otherwise

  */
  bool  startList(const std::string& key, std::istream& is);

  /** reads the value in a key=value pair.

    Reads the value in a 'key=value' pair. Discards any whitespace. Entries
    may be quoted and escaped, enclosing quotes will be removed and escape
    codes will be unescaped.

      @param key const reference to string to match
      @param value reference to the string that will contain the value
      @param is reference to the stream that will be read
      @return bool indicating success or failure
      @see checkKey(), quote(), unEscape()
  */
  bool  readEntry(const std::string& key, std::string& value, std::istream& is);

  /** reads the value in a [ key = "value" ] header.

    like readEntry, but with additional surrounding '[]',
    intended to start top level sections in OM files.

      @param key const reference to string to match
      @param value reference to the string that will contain the value
      @param is reference to the stream that will be read
      @return bool indicating success or failure
      @see readEntry
  */
  bool  readSection(const std::string& key, std::string& value,
      std::istream& is);

  /** reads a list of values in a key={value1 value2 } entry.

    Reads the values in a 'key={value1 value2 ...}' entry. The list is
    whitespace delimited. List entries may be quoted and escaped, enclosing
    quotes will be removed and escape codes will be unescaped. The entries
    in the list will be appended to the values vector.

      @param key const reference to string to match
      @param values reference to the vector of strings that will contain the
      values
      @param is reference to the stream that will be read
      @return bool indicating success or failure
      @see checkKey(), quote(), unEscape()
  */
  bool  readEntry(const std::string& key, std::vector<std::string>& values,
      std::istream& is);

  /** reads the numeric value in a key=value pair.

    Reads the value in a 'key=value' pair. The value has to be numeric.

      @param key const reference to string to match
      @param value reference to the double that will contain the value
      @param is reference to the stream that will be read
      @return bool indicating success or failure
      @see checkKey(), quote()
  */
  bool  readEntry(const std::string& key, double& value, std::istream& is);

  /** reads the numeric value in a key=value pair.

    Reads the value in a 'key=value' pair. The value has to be numeric.

      @param key const reference to string to match
      @param value reference to the int that will contain the value
      @param is reference to the stream that will be read
      @return bool indicating success or failure
      @see checkKey(), quote()
  */
  bool  readEntry(const std::string& key, int& value, std::istream& is);

  /** reads a list of numeric values in a key={value1 value2 } entry.

    Reads the values in a 'key={value1 value2 ...}' entry. The list consists
    of whitespace delimited numvers. The entries in the list will be
    appended to the values vector.

      @param key const reference to string to match
      @param values reference to the vector of doubles that will contain the
      values
      @param is reference to the stream that will be read
      @return bool indicating success or failure
      @see checkKey(), quote()
  */
  bool  readEntry(const std::string& key, std::vector<double>& values,
      std::istream& is);

  /** reads a list of numeric values in a key={value1 value2 } entry.

    Reads the values in a 'key={value1 value2 ...}' entry. The list consists
    of whitespace delimited numvers. The entries in the list will be
    appended to the values vector.

      @param key const reference to string to match
      @param values reference to the vector of ints that will contain the
      values
      @param is reference to the stream that will be read
      @return bool indicating success or failure
      @see checkKey(), quote()
  */
  bool  readEntry(const std::string& key, std::vector<int>& values,
      std::istream& is);

  /** reads a list of values in the format {value1 value2 } entry.

    Reads the values in a '{value1 value2 ...}' list. The list is
    whitespace delimited. List entries may be quoted and escaped, enclosing
    quotes will be removed and escape codes will be unescaped. The entries
    in the list will be appended to the values vector.

      @param values reference to the vector of strings that will contain the
      values
      @param is reference to the stream that will be read
      @return bool indicating success or failure
      @see quote(), unEscape()
  */
  bool  readList(std::vector<std::string>& values, std::istream& is);

  //---------------------------------------------------------------------------
  //@}


  /**@name Streamed output */
  //@{
  //---------------------------------------------------------------------------

  /** write quoted string to a stream.

    Quotes a string and writes it to a stream.

      @param str const reference to string to be written
      @param os reference to the stream that will be written to
      @see quote()
  */
  void  writeQuoted(const std::string& str, std::ostream& os);

  /** writes a key= pair.

    Writes 'key =' is at the current position of the stream.

      @param key const reference to string to write (without the '=')
      @param os reference to the stream that will be write
  */
  void  writeKey(const std::string& key, std::ostream& os);

  /** writes the key and value in a key=value pair.

    Writes a 'key=value' pair. The value will be quoted and escaped.

      @param key const reference to string used as key
      @param value const reference to the string that contains the value
      @param os reference to the stream that will be written to
      @see writeKey(), quote()
  */
  void  writeEntry(const std::string& key, const std::string& value,
      std::ostream& os);

  /** writes a list of values in a #key={value1 value2 }# entry.

    The values will be quoted and escaped.

      @param key const reference to string used as key
      @param values reference to the vector of strings that contain the
      values
      @param os reference to the stream that will be written to
  */
  void  writeEntry(const std::string& key,
      const std::vector<std::string>& values, std::ostream& os);

  /** writes a key=value pair for a numeric value.

    @param key const reference to string used as key
    @param value the double that contains the value
    @param os reference to the stream that will be written to
  */
  void  writeEntry(const std::string& key, double value, std::ostream& os);

  /** writes a key=value pair for a numeric value.

    @param key const reference to string used as key
    @param value the int that contains the value
    @param os reference to the stream that will be written to
  */
  void  writeEntry(const std::string& key, int value, std::ostream& os);

  /** writes a list of numeric values in a #key={value1 value2 }# entry.

    @param key const reference to string used as key
    @param values reference to the vector of doubles that contains the
    values
    @param os reference to the stream that will be written to
  */
  void  writeEntry(const std::string& key, const std::vector<double>& values,
      std::ostream& os);

  /** writes a list of numeric values in a #key={value1 value2 }# entry.

    @param key const reference to string used as key
    @param values reference to the vector of ints that contains the
    values
    @param os reference to the stream that will be written to
  */
  void writeEntry(const std::string& key, const std::vector<int>& values,
      std::ostream& os);

  /**
  * Strips whitespace.
  * @param str The input string.
  */
  std::string  strip(std::string str);

  /** Strips whitespace, the quote characters and whitespace within quotes.
  * Note that this function attempts not to waste any memory.
  * The equivalent java function can be found in common/Algorithm.java
  * @param str The input string. Need not be trimmed already.
  * @param quotes A string containing possible quote characters.
  */
  //std::string  strip(std::string str, std::string quotes);

  /**
  Case insensitive comparison of two standard strings.
  */
  int  compareCaseInsensitive(std::string a, std::string b);

  /// Convert string to lower case
  std::string  toLower(std::string text);

  /// Convert string to upper case
  std::string  toUpper(std::string text);

  /**
    Capitalize each word of the text passed in.

    Capitalizes the first letter in a text string and any other letters in text
    that follow any character other than a letter. Converts all other letters to
    lowercase letters.

    Examples:

    properCase("this is a TITLE") equals "This Is A Title"

    properCase("2-cent's worth") equals "2-Cent'S Worth"

    properCase("76BudGet") equals "76Budget"
  */
  std::string  toProperCase(std::string text);

  /**
    Returns the part of @a str that matches @passThrough in @a mask.

    For example, if @mask is @C "YYYYMMDD", @a passThrough is @c 'Y' and @a str
    is @c "20041201", the return value is @c "2004".

    @note The occurrences of @a passThrough need not be adjacent; if @a mask
    where @c "YDY" and @a passThrough @c 'Y' then the first and third character
    of @str would be returned.
   */
  std::string  maskedSubstr(std::string str, std::string mask, char passThrough);

  /**
    Strips matching - but not themselves quoted - quotes (' or ") from @a str.

    For example:
    @code
    removeMatchingQuotes("'foo'"); // yields foo
    removeMatchingQuotes("'\"foo\"'"); // yields "foo"
    removeMatchingQuotes("\"'foo'\""); // yields 'foo'
    removeMatchingQuotes("'foo\""); // yields 'foo"
    removeMatchingQuotes("'foo"); // yields 'foo
    @endcode

   */
  std::string  removeMatchingQuotes(std::string str);

  /**
    Removes all occurences of @padding from the left of @str.
   */
  std::string  stripLeft(std::string str, char padding);
  /**
    Removes all occurences of @padding from the right of @str.
   */
  std::string  stripRight(std::string str, char padding);
  /**
    Removes all occurences of @padding from the left and right of @str.
   */
  std::string  strip(std::string str, char padding);

  /**
    Removes all whitespace from the left of @str.
   */
  std::string  stripLeft(std::string str);
  /**
    Removes all whitespace from the right of @str.
   */
  std::string  stripRight(std::string str);
  /**
    Removes all whitespace from the left and right of @str.
   */
  std::string  strip(std::string str);
  /**
	Return the string from the beginning to the first occurence of the "padding" character (not included)
	(added by nicolas 20090331)
  */
  std::string stripAfter(std::string str, char padding);


//@}

//---------------------------------------------------------------------------
}
#endif

