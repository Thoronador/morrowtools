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

#ifndef COMPILEDCHUNK_H
#define COMPILEDCHUNK_H

#include <vector>
#include <string>
#include <stdint.h>

namespace MWTP
{

namespace ScriptCompiler
{

enum SC_VarType {vtShort, vtLong, vtFloat, vtGlobal};

struct SC_VarRef
{
  SC_VarType Type;
  uint16_t Index;

  /* constructor */
  SC_VarRef(const SC_VarType t, const uint16_t i);

  /* default constructor */
  SC_VarRef();
};//struct

/* class to basically hold an array/vector of bytes */
struct CompiledChunk
{
  //the byte data
  std::vector<uint8_t> data;

  //the list of local variables - split by data types
  std::vector<std::string> varsShort;
  std::vector<std::string> varsLong;
  std::vector<std::string> varsFloat;

  /* constructor */
  CompiledChunk();

  /* pushes an op-code onto the back of the data

     parameters:
         code - the op-code
  */
  void pushCode(const uint16_t code);

  /* pushes a short value onto the back of the data

     parameters:
         value - the short value
  */
  void pushShort(const int16_t value);

  /* pushes a long value onto the back of the data

     parameters:
         value - the long value
  */
  void pushLong(const int32_t value);

  /* pushes a float value onto the back of the data

     parameters:
         value - the floating point
  */
  void pushFloat(const float value);

  /* pushes a string onto the back of the data

     parameters:
         str - the string
  */
  void pushString(const std::string& str);

  /* pushes a non-global reference to a local var with 0x0000 as filler

     parameters:
         ref - the variable reference
  */
  void pushNonGlobalRefWithTwoZeroFillers(const SC_VarRef& ref);

  /* pushes a non-global reference to a local var, no fillers

     parameters:
         ref - the variable reference
  */
  void pushNonGlobalRef(const SC_VarRef& ref);

  /* checks whether varName does name a local var of the script */
  SC_VarRef getVariableTypeWithIndex(const std::string& varName) const;
}; //struct

} //namespace ScriptCompiler

} //namespace MWTP

#endif // COMPILEDCHUNK_H
