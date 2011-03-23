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

#include "CompiledChunk.h"
#include <iostream>
#include "UtilityFunctions.h"

namespace ScriptCompiler
{

SC_VarRef::SC_VarRef(const SC_VarType t, const uint16_t i)
{
  Type = t;
  Index = i;
}

void CompiledChunk::pushCode(const uint16_t code)
{
  data.push_back(code&255);
  data.push_back(code>>8);
}

void CompiledChunk::pushShort(const int16_t value)
{
  data.push_back(value&255);
  data.push_back(value>>8);
}

void CompiledChunk::pushLong(const int32_t value)
{
  data.push_back(value&255);
  data.push_back((value>>8)&255);
  data.push_back((value>>16)&255);
  data.push_back(value>>24);
}

void CompiledChunk::pushFloat(const float value)
{
  const int32_t * ptr = (int32_t*) &value;
  data.push_back(*ptr & 255);
  data.push_back((*ptr>>8)&255);
  data.push_back((*ptr>>16)&255);
  data.push_back((*ptr>>24)&255);
}

void CompiledChunk::pushString(const std::string& str)
{
  unsigned int i;
  const unsigned int len = str.length();
  for (i=0; i<len; ++i)
  {
    data.push_back(str[i]);
  }//for
}

void CompiledChunk::pushNonGlobalRefWithTwoZeroFillers(const SC_VarRef& ref)
{
  if (ref.Type==vtGlobal)
  {
    std::cout << "Script Compiler: Error: Trying to push global ref, but that's"
              << " not allowed here.\n";
    throw 42;
    return;
  }

  switch (ref.Type)
  {
    case vtShort:
         //push s for short
         data.push_back('s');
         break;
    case vtLong:
         //push l for long
         data.push_back('l');
         break;
    case vtFloat:
         //push f for float
         data.push_back('f');
         break;
    case vtGlobal:
         //This branch is only here to shut up the compiler warning, but it will
         // never be reached. (vtGlobal is checked above.)
         throw 42;
         break;
  }//swi
  //push index
  data.push_back(ref.Index);
  //push fillers
  data.push_back(0);
  data.push_back(0);
  return;
}

SC_VarRef CompiledChunk::getVariableTypeWithIndex(const std::string& varName) const
{
  const std::string lcVar = lowerCase(varName);
  unsigned int i;
  //check list of shorts
  for (i=0; i<varsShort.size(); ++i)
  {
    if (lcVar==lowerCase(varsShort.at(i)))
    {
      return SC_VarRef(vtShort, i+1);
    }
  }//for
  //check list of longs
  for (i=0; i<varsLong.size(); ++i)
  {
    if (lcVar==lowerCase(varsLong.at(i)))
    {
      return SC_VarRef(vtLong, i+1);
    }
  }//for
  //check list of floats
  for (i=0; i<varsFloat.size(); ++i)
  {
    if (lcVar==lowerCase(varsFloat.at(i)))
    {
      return SC_VarRef(vtFloat, i+1);
    }
  }//for
  //if we get to this point, nothing was found and it has to be a non-local var,
  // i.e. a global (or some other kind of expression we can't identify yet).
  return SC_VarRef(vtGlobal, 0);
}

} //namespace
