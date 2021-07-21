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

#ifndef MW_PATHGRIDRECORD_HPP
#define MW_PATHGRIDRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"

namespace MWTP
{

struct GridPointData
{
  int32_t X;
  int32_t Y;
  int32_t Z;
  int32_t Unknown;

  /* equality operator */
  bool operator==(const GridPointData& other) const;
};//struct

struct GridConnection
{
  uint32_t Start;
  uint32_t End;

  /* equality operator */
  bool operator==(const GridConnection& other) const;
};//struct

struct PathGridRecord: public BasicRecord
{
  //path grid data
  int32_t GridX;
  int32_t GridY;
  uint16_t Granularity;
  uint16_t NumQuads;
  //end of path grid data
  std::string CellName;
  std::vector<GridPointData> Points;
  std::vector<GridConnection> Connections;

  /* constructor */
  PathGridRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const PathGridRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the record to the given output stream and returns true on success

    parameters:
        output   - the output file stream
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

#endif // MW_PATHGRIDRECORD_HPP
