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

#ifndef SCRIPTCOMPILER_H
#define SCRIPTCOMPILER_H

#include "../records/ScriptRecord.h"
#include <vector>

namespace ScriptCompiler
{

/* class to basically hold an array/vector of bytes */
struct CompiledChunk
{
  //the byte data
  std::vector<uint8_t> data;

  /* pushes an op-code onto the back of the data

     parameters:
         code - the op-code
  */
  void pushCode(const uint16_t code);

  /* pushes a string onto the back of the data

     parameters:
         str - the string
  */
  void pushString(const std::string& str);
}; //struct

enum SC_VarType {vtShort, vtLong, vtFloat, vtGlobal};

struct SC_VarRef
{
  SC_VarType Type;
  uint16_t Index;

  /* constructor */
  SC_VarRef(const SC_VarType t, const uint16_t i);
};//struct

/* removes all leading and trailing spaces and (horizontal) tabulators from the
   given string

   parameters:
       str1 - the affected string

*/
void trim(std::string& str1);

/* removes the first and the last character of the given string, if they both
   are double quotes. For example, '"ABC"' becomes 'ABC'.

   parameters:
       str1 - the affected string

*/
void StripEnclosingQuotes(std::string& str1);

/* This function tries to "compile" the script given in text and stores the
   compiled script in result. If the function succeeded, true is returned. If an
   error occured, false is returned. In the later case, the state of result
   has not changed.

   parameters:
       Text   - the text of the script
       result - the ScriptRecord structure that will hold the processed script

   remarks:
       This function is still VERY far from complete. Currently everything
       besides variable declarations is ignored. Hence, result will most likely
       NOT contain the data you want - yet.
*/
bool CompileScript(const std::string& Text, ScriptRecord& result);

}//namespace

#endif // SCRIPTCOMPILER_H
