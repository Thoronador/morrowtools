/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "XMLParser.h"

XMLParser::XMLParser()
: m_Buffer("")
{ }

XMLParser::~XMLParser()
{
  m_Buffer.clear();
}

void XMLParser::feed(const std::string& next)
{
  m_Buffer.append(next);
}

bool XMLParser::hasEmptyBuffer() const
{
  return m_Buffer.empty();
}

void XMLParser::clearBuffer()
{
  m_Buffer.clear();
}

ParserToken XMLParser::parse(std::string& Content)
{
  if (m_Buffer.empty())
  {
    return ptNone;
  }
  std::string::size_type len = m_Buffer.length();
  std::string::size_type look = 0;
  //remove spaces (i.e. space, tab, line feed, carriage return
  bool go_on = true;
  while (go_on)
  {
    if (isXMLSpace(m_Buffer.at(look)))
    {
      ++look;
      go_on = (look<len);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (look>=len)
  {
    m_Buffer = "";
    return ptNone;
  }
  m_Buffer.erase(0, look);
  //shouldn't be empty, so go on
  len = m_Buffer.length();
  if (m_Buffer.at(0)=='<')
  {
    //possible tag start
    ParserToken result = ptOpeningTag;
    if (len>1)
    {
      if (isspace(m_Buffer.at(1)))
      {
        //shouldn't be space but alphanumeric
        Content = "Error: Space after tag start!";
        return ptError;
      }
      if (m_Buffer.at(1)=='/')
      {
        result = ptClosingTag;
      }
    }//if len>1

    //search for end of tag
    go_on = true;
    look = 0;
    while (go_on)
    {
      if (m_Buffer.at(look)=='>')
      {
        //tag ends here
        Content = m_Buffer.substr(1, look-1);
        //remove closing tag's slash
        if (result==ptClosingTag)
        {
          Content.erase(0,1);
        }
        //delete processed stuff from buffer
        m_Buffer.erase(0, look+1);
        return result;
      }
      ++look;
      go_on = (look<len);
    }//while
    //no > found, so buffer is too short yet
    return ptNone;
  }//if possible opening tag found
  else
  {
    //probably text only, so go on until next tag start
    look = 0;
    go_on = true;
    while (go_on)
    {
      if (m_Buffer.at(look)=='<')
      {
        //text ends here
        Content = m_Buffer.substr(0, look);
        m_Buffer.erase(0, look);
        return ptText;
      }
      ++look;
      go_on = (look<len);
    }//while
    //all stuff seems to be text, get it
    Content = m_Buffer;
    m_Buffer.clear();
    return ptText;
  }
}

bool XMLParser::isXMLSpace(const char c) const
{
  return ((c==' ') or (c=='\t') or (c=='\r') or (c=='\n'));
}
