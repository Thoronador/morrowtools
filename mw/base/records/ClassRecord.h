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

#ifndef MW_CLASSRECORD_H
#define MW_CLASSRECORD_H

#include <string>
#include <fstream>
#include "BasicRecord.h"

namespace MWTP
{

struct ClassRecord: public BasicRecord
{
  std::string ClassID;
  std::string Name;
  //CLDT
  int32_t AttrID1;
  int32_t AttrID2;
  int32_t Specialization;
  int32_t MinorID1;
  int32_t MajorID1;
  int32_t MinorID2;
  int32_t MajorID2;
  int32_t MinorID3;
  int32_t MajorID3;
  int32_t MinorID4;
  int32_t MajorID4;
  int32_t MinorID5;
  int32_t MajorID5;
  int32_t ClassFlags;
  int32_t AutoCalcFlags;
  //end of CLDT
  std::string Description;

  /* constructor */
  ClassRecord();

  /* alternative constructor */
  ClassRecord(const std::string& ID);

  /* returns true, if the other record contains the same data */
  bool equals(const ClassRecord& other) const;

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

#endif // MW_CLASSRECORD_H
