//
//  Rev:          $Id: Properties.h,v 1.4 2005/03/02 12:37:21 evert Exp $
//
//  Author:       Evert Haasdijk
//  Contact:      evert@zukkespijkers.nl
//
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

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <iostream>
#include <map>
#include <vector>
#include <string>

//#include <zsu/config.h>

//---------------------------------------------------------------------------
namespace zsu
{

/**
 * The <code>Properties</code> class represents a persistent set of
 * properties. The <code>Properties</code> can be saved to a stream
 * or loaded from a stream. Each key and its corresponding value in
 * the property list is a std::string.
 * <p>
 * A property list can contain another property list as its
 * "defaults"; this second property list is searched if
 * the property key is not found in the original property list.
 * <p>
 * Because <code>Properties</code> inherits from <code>map&lt;std::string,std::string&gt;</code>,
 * all <code>map</code> methods can be applied to a
 * <code>Properties</code> object.  Their use is strongly discouraged as they
 * allow the caller to insert entries whose keys or values are not
 * <code>std::strings</code>.  The <code>setProperty</code> method should be used
 * instead.  If the <code>store</code> or <code>save</code> method is called
 * on a "compromised" <code>Properties</code> object that contains a
 * non-<code>std::string</code> key or value, the call will fail.
 */
class  Properties :
public std::map<std::string, std::string> {
public:
    /**
     * Creates an empty property list with no default values. Use the
     * standard available copy-constructor to create one with defaults
     * of the given <code>Properties</code> object.
     */
    Properties();

    /**
     * Adds a new property. Alternative way is to use <code>map</code>
     * <code>operator []</code>. Provided for
     * parallelism with the <tt>getProperty</tt> method. Enforces use of
     * std::strings for property keys and values.
     */
    void setProperty(std::string key, std::string value);

    /**
     * Reads a property list (key and element pairs) from the input stream.
     * <p>
     * Every property occupies one line of the input stream. Each line
     * is terminated by a line terminator (<code>\\n</code> or <code>\\r</code>
     * or <code>\\r\\n</code>). Lines from the input stream are processed until
     * end of file is reached on the input stream.
     * <p>
     * A line that contains only whitespace or whose first non-whitespace
     * character is an ASCII <code>#</code> or <code>!</code> is ignored
     * (thus, <code>#</code> or <code>!</code> indicate comment lines).
     * <p>
     * Every line other than a blank line or a comment line describes one
     * property to be added to the table (except that if a line ends with \,
     * then the following line, if it exists, is treated as a continuation
     * line, as described
     * below). The key consists of all the characters in the line starting
     * with the first non-whitespace character and up to, but not including,
     * the first ASCII <code>=</code>, <code>:</code>, or whitespace
     * character. All of the key termination characters may be included in
     * the key by preceding them with a \.
     * Any whitespace after the key is skipped; if the first non-whitespace
     * character after the key is <code>=</code> or <code>:</code>, then it
     * is ignored and any whitespace characters after it are also skipped.
     * All remaining characters on the line become part of the associated
     * element string. Within the element string, the ASCII
     * escape sequences <code>\\t</code>, <code>\\n</code>,
     * <code>\\r</code>, <code>\\\\</code>, <code>\\"</code>, <code>\\'</code>,
     * <code>\ &#32;</code> &#32;(a backslash and a space),
     * are recognized and converted to single
     * characters. Moreover, if the last character on the line is
     * <code>\\</code>, then the next line is treated as a continuation of the
     * current line; the <code>\\</code> and line terminator are simply
     * discarded, and any leading whitespace characters on the continuation
     * line are also discarded and are not part of the element string.
     * <p>
     * As an example, each of the following four lines specifies the key
     * <code>"Truth"</code> and the associated element value
     * <code>"Beauty"</code>:
     * <p>
     * <pre>
     * Truth = Beauty
     *	Truth:Beauty
     * Truth			:Beauty
     * </pre>
     * As another example, the following three lines specify a single
     * property:
     * <p>
     * <pre>
     * fruits				apple, banana, pear, \\
     *                                  cantaloupe, watermelon, \\
     *                                  kiwi, mango
     * </pre>
     * The key is <code>"fruits"</code> and the associated element is:
     * <p>
     * <pre>"apple, banana, pear, cantaloupe, watermelon,kiwi, mango"</pre>
     * Note that a space appears before each <code>\\</code> so that a space
     * will appear after each comma in the final result; the <code>\\</code>,
     * line terminator, and leading whitespace on the continuation line are
     * merely discarded and are <i>not</i> replaced by one or more other
     * characters.
     * <p>
     * As a third example, the line:
     * <p>
     * <pre>cheeses
     * </pre>
     * specifies that the key is <code>"cheeses"</code> and the associated
     * element is the empty string.<p>
     *
     * @param      inStream   the input stream.
     */
    void load(std::istream& inStream);

    /**
     * Writes this property list (key and element pairs) in this
     * <code>Properties</code> table to the output stream in a format suitable
     * for loading into a <code>Properties</code> table using the
     * <code>load</code> method.
     * <p>
     * Properties from the defaults table of this <code>Properties</code>
     * table (if any) are <i>not</i> written out by this method.
     * <p>
     * If the header argument is not null, then an ASCII <code>#</code>
     * character, the header string, and a line separator are first written
     * to the output stream. Thus, the <code>header</code> can serve as an
     * identifying comment.
     * <p>
     * Next, a comment line is always written, consisting of an ASCII
     * <code>#</code> character, the current date and time (as if produced
     * by the <code>tostring</code> method of <code>Date</code> for the
     * current time), and a line separator.
     * <p>
     * Then every entry in this <code>Properties</code> table is written out,
     * one per line. For each entry the key string is written, then an ASCII
     * <code>=</code>, then the associated element string. Each character of
     * the element string is examined to see whether it should be rendered as
     * an escape sequence. The ASCII characters <code>\\</code>, tab, newline,
     * and carriage return are written as <code>\\\\</code>, <code>\\t</code>,
     * <code>\\n</code>, and <code>\\r</code>, respectively.
     * Leading space characters,
     * but not embedded or trailing space characters, are written with a
     * preceding <code>\\</code>. The key and value characters <code>#</code>,
     * <code>!</code>, <code>=</code>, and <code>:</code> are written with a
     * preceding slash to ensure that they are properly loaded.
     * <p>
     * After the entries have been written, the output stream is flushed.  The
     * output stream remains open after this method returns.
     *
     * @param   out      an output stream.
     * @param   header   a description of the property list.
     */
    void store(std::ostream& out, std::string header);

    /**
     * Same without a header. Coded separately to avoid default arguments,
     * which are not handy when creating pre-compiled headers.
     */
    void store(std::ostream& out);

    /**
     * Searches for the property with the specified key in this property list.
     * If the key is not found in this property list, the default property list,
     * and its defaults, recursively, are then checked. The method returns
     * an empty string if the property is not found.
     *
     * @param   key   the property key.
     * @return  the value in this property list with the specified key value.
     * @see     #setProperty
     */
    std::string getProperty(std::string key);

    /**
     * Searches for the property with the specified key in this property list.
     * If the key is not found in this property list, the default property list,
     * and its defaults, recursively, are then checked. The method returns the
     * default value argument if the property is not found.
     *
     * @param   key            the hashtable key.
     * @param   defaultValue   a default value.
     *
     * @return  the value in this property list with the specified key value.
     * @see     #setProperty
     */
    std::string getProperty(std::string key, std::string defaultValue);

    /**
     * Checks if a property of the given name exists.
     * If the key is not found in this property list, the default property list,
     * and its defaults, recursively, are then checked.
     *
     * @param   key   the property key.
     * @return  true if the property exists, false otherwise.
     */
	  bool hasProperty(std::string key);

    /**
     * Add properties from command-line arguments. Existing values of
     * properties will be overwritten. Command line arguments have two
     * forms:
     * <pre>
     *    -f \<file\>
     * </pre>
     * which reads a whole set of properties from the file given. The
     * other form is to define properties directly on the command-line:
     * <pre>
     *    -\<prop\> \<value\>
     * </pre>
     * which defines a value for a single property.
     * <p>
     * Command-line arguments are processed from left to right. Any
     * command-line arguments that have not been used (which don't follow
     * the syntax above), will be returned as a vector of strings.
     *
     * @param argc The number of command-line args as standard in C/C++.
     * @param argv The array with command-line args as standard in C/C++.
     * @return A vector of unused command-line args, converted to strings.
     */
    std::vector<std::string> addCommandLineArgs(int argc, char* argv[]);

    /**
      Tries to load \c property from the environment.

      For example, to load the PATH environment variable as a property:
      <pre>
        zsu::Properties p;
        p.addEnvironment("PATH");
        std::cout << "PATH=" << p["PATH"];
      </pre>    
     */
    void addEnvironment(std::string property);
    
    /**
      Loads properties from a string with \c prop=value pairs.

      Entries are whitespace delimited, but may be quoted with ". Whitespace
      around the \c '=' is disregarded (so \c foo=bar and \c foo \c = \c bar
      both yield a property \c 'foo' with the value \c 'bar'.

      Any substrings not part of a name-value pair are discarded, so
      \c foo \c bar \c = \c pi yields a single property \c 'bar' with value
      \c 'pi'.
    */
    void load(std::string props);

private:

  /**
    Returns \c true if the given line must be appended to the next line (i.e.
    if it ends in an even number of backslashes).
    Sideeffect: if it returns \c true, the trailing backslashes are removed.
  */
	bool continueLine(std::string& line);
  
  /**
  * Converts encoded &#92;uxxxx to unicode chars
  * and changes special saved chars to their original forms
  */
	std::string loadConvert(std::string thestring);

  /**
  * Converts unicodes to encoded &#92;uxxxx
  * and writes out any of the characters in specialSaveChars
  * with a preceding slash
  */
	std::string saveConvert(std::string thestring, bool escapeSpace);

  /// Helper func to read a string, whitespace delimited but possibly quoted
  std::string readString(std::istream& is);

  /// Helper func to read 'name=value' from a whitespace-delimited sequence
  std::pair<std::string, std::string> readPropertyEntry(std::istream& is);
};
//---------------------------------------------------------------------------
} // end namespace

//---------------------------------------------------------------------------
#endif



