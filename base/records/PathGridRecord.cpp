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

#include "PathGridRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

bool GridPointData::operator==(const GridPointData& other) const
{
  return ((X==other.X) and (Y==other.Y) and (Z==other.Z)
      and (Unknown==other.Unknown));
}

bool GridConnection::operator==(const GridConnection& other) const
{
  return ((Start==other.Start) and (End==other.End));
}

PathGridRecord::PathGridRecord()
{
  GridX = GridY = 0;
  Granularity = 0;
  NumQuads = 0;
  CellName = "";
  Points.clear();
  Connections.clear();
}

bool PathGridRecord::equals(const PathGridRecord& other) const
{
  return ((GridX==other.GridX) and (GridY==other.GridY)
      and (Granularity==other.Granularity) and (NumQuads==other.NumQuads)
      and (CellName==other.CellName) and (Points==other.Points)
      and (Connections==other.Connections));
}

bool PathGridRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cPGRD, 4);
  uint32_t Size;
  Size = 4 /* DATA */ +4 /* 4 bytes for length */ + 12 /* size of data */
        +4 /* NAME */ +4 /* 4 bytes for length */
        +CellName.length()+1 /* length of cell name +1 byte for NUL termination */;
  if (!Points.empty())
  {
    Size = Size + 4 /* PGRP */ +4 /* 4 bytes for length */
           +16 * Points.size();
  }
  if (!Connections.empty())
  {
    Size = Size + 4 /* PGRC */ +4 /* 4 bytes for length */
           +8 * Connections.size();
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Path Grid:
    DATA = (path grid data, 12 bytes)
             int32_t GridX
             int32_t GridY
             uint16_t Granularity (a power of two within [128;4096])
             uint16_t NumQuads (number of points in PGRP)
    NAME = name of cell the grid belongs to
    PGRP = unknown data (path grid points?)
             looks like an array of int32_t quads
             struct PointData
             {
               int32_t X
               int32_t Y
               int32_t Z
               int32_t Unknown
             };

    PGRC = unknown data (length: ?)
             Possibly path grid connections?
             Looks like an array of uint32_t pairs, where
             first value is the index of the starting point
             and second value is index of the end point.
  */

  //write DATA
  output.write((char*) &cDATA, 4);
  //DATA's length
  uint32_t SubLength;
  SubLength = 12; //length is always twelve bytes
  output.write((char*) &SubLength, 4);
  //write path grid data
  output.write((char*) &GridX, 4);
  output.write((char*) &GridY, 4);
  output.write((char*) &Granularity, 2);
  output.write((char*) &NumQuads, 2);

  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  SubLength = CellName.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write cell name
  output.write(CellName.c_str(), SubLength);

  if (!Points.empty())
  {
    //write PGRP
    output.write((char*) &cPGRP, 4);
    //PGRP's length
    SubLength = Points.size()*16;//length is 16 bytes per point
    output.write((char*) &SubLength, 4);
    //write points
    unsigned int i;
    for (i=0; i<Points.size(); ++i)
    {
      //write next point
      output.write((char*) &(Points[i].X), 4);
      output.write((char*) &(Points[i].Y), 4);
      output.write((char*) &(Points[i].Z), 4);
      output.write((char*) &(Points[i].Unknown), 4);
    }//for
  }//Points

  if (!Connections.empty())
  {
    //write PGRC
    output.write((char*) &cPGRC, 4);
    //PGRC's length
    SubLength = Connections.size()*8;//length is 8 bytes per connection
    output.write((char*) &SubLength, 4);
    //write connections
    unsigned int i;
    for (i=0; i<Connections.size(); ++i)
    {
      //write next point
      output.write((char*) &(Connections[i].Start), 4);
      output.write((char*) &(Connections[i].End), 4);
    }//for
  }//connections

  return output.good();
}

bool PathGridRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Path Grid:
    DATA = (path grid data, 12 bytes)
             int32_t GridX
             int32_t GridY
             uint16_t Granularity (a power of two within [128;4096])
             uint16_t NumQuads (number of points in PGRP)
    NAME = name of cell the grid belongs to
    PGRP = unknown data (path grid points?)
             looks like an array of int32_t quads
             struct PointData
             {
               int32_t X
               int32_t Y
               int32_t Z
               int32_t Unknown
             };

    PGRC = unknown data (length: ?)
             Possibly path grid connections?
             Looks like an array of uint32_t pairs, where
             first value is the index of the starting point
             and second value is index of the end point.
  */

  int32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=12)
  {
    std::cout << "Error: subrecord DATA of PGRD is has invalid length ("
              << SubLength<<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read path grid's data
  in_File.read((char*) &GridX, 4);
  in_File.read((char*) &GridY, 4);
  in_File.read((char*) &Granularity, 2);
  in_File.read((char*) &NumQuads, 2);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of PGRD.\n";
    return false;
  }

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME of PGRD is longer than 255 characters.\n";
    return false;
  }
  //read cell ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of PGRD.\n";
    return false;
  }
  CellName = std::string(Buffer);

  Points.clear();
  Connections.clear();
  while (BytesRead<Size)
  {
    //read PGRP or PGRC
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName==cPGRP)
    {
      //PGRP's length
      in_File.read((char*) &SubLength, 4);
      BytesRead += 4;
      if (SubLength<0)
      {
        std::cout << "Error: subrecord PGRP of PGRD has invalid length.\n";
        return false;
      }
      if ((SubLength%16)!=0)
      {
        std::cout << "Error: subrecord PGRP of PGRD has invalid length ("<<SubLength
                  << " bytes). Should be a multiple of 16 bytes.\n";
        return false;
      }
      //read PGRP
      const unsigned int point_count = SubLength / 16;
      GridPointData temp_pt;
      while (Points.size()<point_count)
      {
        //read next point
        in_File.read((char*) &(temp_pt.X), 4);
        in_File.read((char*) &(temp_pt.Y), 4);
        in_File.read((char*) &(temp_pt.Z), 4);
        in_File.read((char*) &(temp_pt.Unknown), 4);
        BytesRead += 16;
        if (!in_File.good())
        {
          std::cout << "Error while reading subrecord PGRP of PGRD.\n";
          return false;
        }
        Points.push_back(temp_pt);
      }//while
    }//if PGRP
    else if (SubRecName==cPGRC)
    {
      //PGRC's length
      in_File.read((char*) &SubLength, 4);
      BytesRead += 4;
      if (SubLength<0)
      {
        std::cout << "Error: subrecord PGRC of PGRD has invalid length.\n";
        return false;
      }
      /*if ((SubLength%8)!=0)
      {
        std::cout << "Warning: subrecord PGRC of PGRD has invalid length ("<<SubLength
                  << " bytes). Should be a multiple of 8 bytes. The "<<(SubLength%8)
                  << " byte(s) at the end will be discarded.\n";
      }*/
      //read PGRC
      const unsigned int connect_count = SubLength / 8;
      GridConnection temp_con;
      while (Connections.size()<connect_count)
      {
        //read next connection
        in_File.read((char*) &(temp_con.Start), 4);
        in_File.read((char*) &(temp_con.End), 4);
        BytesRead += 8;
        if (!in_File.good())
        {
          std::cout << "Error while reading subrecord PGRC of PGRD.\n";
          return false;
        }
        Connections.push_back(temp_con);
      }//while
      //It's a bit of a dirty trick to skip the remainder, but I don't see why
      // there should be an incomplete connection with just one point.
      if ((SubLength%8)!=0)
      {
        in_File.seekg(SubLength%8, std::ios_base::cur);
        BytesRead += (SubLength%8);
      }
    }//if PGRC
    else
    {
      //unexpected record found
      std::cout << "Error: expected record name PGRP or PGRC was not found. "
                << "Instead, \""<<IntTo4Char(SubRecName)<<"\" was found.\n";
      return false;
    }
  }//while BytesRead<Size
  return in_File.good();
}
