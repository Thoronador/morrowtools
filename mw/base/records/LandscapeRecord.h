/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef MW_LANDSCAPERECORD_H
#define MW_LANDSCAPERECORD_H

#include <fstream>
#include "BasicRecord.h"

namespace MWTP
{

struct LandscapeRecord: public BasicRecord
{
  int32_t CellX;
  int32_t CellY;
  int32_t UnknownData;
  bool hasNormals;
  int8_t Normals[65][65][3];
  bool hasHeightSubrecord;
  float HeightOffset;
  uint8_t UnknownByte;
  int8_t HeightData[65][65];
  int16_t UnknownShort;
  bool hasWNAMSubrecord;
  char Data[81];
  //optional parts
  bool hasVertexColour;
  int8_t VertexColour[65][65][3];
  bool hasVertexTextures;
  int16_t VertexTextures[16][16];

  /* constructor */
  LandscapeRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const LandscapeRecord& other) const;

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

#endif // MW_LANDSCAPERECORD_H
