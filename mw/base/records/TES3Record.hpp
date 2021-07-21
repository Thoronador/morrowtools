/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef MW_TES3RECORD_H
#define MW_TES3RECORD_H

#include "BasicRecord.hpp"
#include <string>
#include "../DepFiles.hpp"

namespace MWTP
{

struct TES3Record: public BasicRecord
{
  //subrecord HEDR
  float Version;
  uint32_t FileFlag;
  std::string companyName;
  std::string description;
  uint32_t NumRecords;
  //end of subrecord HEDR
  DepFileList dependencies;


  /* constructor */
  TES3Record();

  /* returns true, if the other record contains the same data */
  bool equals(const TES3Record& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output   - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};//struct

} //namespace

#endif // MW_TES3RECORD_H
