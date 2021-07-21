/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef MW_SCRIPTRECORD_HPP
#define MW_SCRIPTRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"

namespace MWTP
{

struct ScriptRecord: public BasicRecord
{
  //script header
  std::string recordID; //formerly ScriptID
  uint32_t NumShorts;
  uint32_t NumLongs;
  uint32_t NumFloats;
  uint32_t ScriptDataSize;
  int32_t LocalVarSize;
  //end of script header
  std::vector<std::string> LocalVars;
  char * ScriptData;
  std::string ScriptText;

  /* constructor */
  ScriptRecord();

  /* copy constructor */
  ScriptRecord(const ScriptRecord& source);

  /* assignment operator */
  ScriptRecord& operator=(const ScriptRecord& rhs);

  /* destructor */
  virtual ~ScriptRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const ScriptRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;
  #endif

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};//struct

} //namespace

#endif // MW_SCRIPTRECORD_HPP
