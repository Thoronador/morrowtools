/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>

enum ParserToken {ptNone, ptError, ptOpeningTag, ptClosingTag, ptText};

/* The XMLParser class provides a very incomplete(!) parser for XML. It does
   NOT recognize attributes yet, does not handle entities like &amp; and won't
   handle PI at all. DTDs are also not recognized, and it might also not be able
   to handle even some more stuff. You've been warned!
*/

class XMLParser
{
  public:
    /* constructor */
    XMLParser();

    /* destructor */
    ~XMLParser();

    /* feeds new data to the parser

       parameters:
           next - the string containing the next bit of the XML source text
    */
    void feed(const std::string& next);

    /* returns true, if the internal buffer is empty */
    bool hasEmptyBuffer() const;

    /* clears the internal buffer. (You should probably use this after parse()
       returned ptError.)
    */
    void clearBuffer();

    /* tries to parse a bit from the fed data and returns the type of data that
       could have been parsed. If the result is not ptNone or ptError, Content
       will be set to the name of the tag or the content of the text element.

       parameters:
           Content - string that is used to return the associated name
    */
    ParserToken parse(std::string& Content);
  private:
    /* returns true, if the character c is a space character as per XML spec.,
       i.e. if it is a space, a line feed, carriage return or horzontal
       tabulator

       parameters:
           c - the character
    */
    bool isXMLSpace(const char c) const;

    /* internal buffer */
    std::string m_Buffer;
};//class

#endif // XMLPARSER_H
