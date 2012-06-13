/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "Tokenizer.h"

namespace MWTP
{

namespace ScriptCompiler
{

Tokenizer::Tokenizer()
{

}

Tokenizer::~Tokenizer()
{
  //empty
}

void Tokenizer::tokenize(std::string code)
{
  if (code.empty()) return;

  bool inQuotes = false;

  std::string::size_type pos = 0;
  while (pos<code.length())
  {
    if (isSpace(code.at(pos)) and not inQuotes)
    {
      //split
      if (pos!=0)
      {
        m_Tokens.push_back(code.substr(0, pos));
      }
      code = code.substr(pos+1);
      pos = 0;
    }
    else if (code.at(pos)=='"')
    {
      inQuotes = not inQuotes;
      if (not inQuotes)
      {
        m_Tokens.push_back(code.substr(0, pos+1));
        code = code.substr(pos+1);
        pos = 0;
      }
      else
      {
        ++pos;
      }
    }
    else
    {
      ++pos;
    }
  }//while
}

const std::vector<std::string>& Tokenizer::getTokens() const
{
  return m_Tokens;
}

void Tokenizer::clearTokens()
{
  m_Tokens.clear();
}

bool Tokenizer::isSpace(const char c) const
{
  return ((c==' ') or (c=='\n') or (c=='\r') or (c=='\t'));
}

} //namespace ScriptCompiler

} //namespace MWTP
