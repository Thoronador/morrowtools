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

#ifndef MW_TOKENIZER_H
#define MW_TOKENIZER_H

#include <string>
#include <vector>

namespace MWTP
{

namespace ScriptCompiler
{

class Tokenizer
{
  public:
    /* constructor */
    Tokenizer();

    /* destructor */
    ~Tokenizer();

    /* splits the given script code into tokens */
    void tokenize(std::string code);

    /* returns the current list of tokens */
    const std::vector<std::string>& getTokens() const;

    /* clears the current token list */
    void clearTokens();
  private:
    bool isSpace(const char c) const;

    std::vector<std::string> m_Tokens;
};//class

} //namespace ScriptCompiler

} //namespace MWTP

#endif // MW_TOKENIZER_H
