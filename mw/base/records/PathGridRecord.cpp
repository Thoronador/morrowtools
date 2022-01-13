/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2022  Dirk Stolle

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

#include "PathGridRecord.hpp"
#include <numeric>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

GridPointData::GridPointData()
: X(0),
  Y(0),
  Z(0),
  Flags(0),
  ConnectionCount(0),
  Unknown(0)
{
}

bool GridPointData::operator==(const GridPointData& other) const
{
  return (X == other.X) && (Y == other.Y) && (Z == other.Z)
      && (Flags == other.Flags) && (ConnectionCount == other.ConnectionCount)
      && (Unknown == other.Unknown);
}

GridConnection::GridConnection()
: Start(0),
  End(0)
{
}

bool GridConnection::operator==(const GridConnection& other) const
{
  return (Start == other.Start) && (End == other.End);
}

PathGridRecord::PathGridRecord()
: BasicRecord(),
  GridX(0), GridY(0),
  Granularity(0),
  CellName(""),
  Points(std::vector<GridPointData>()),
  Connections(std::vector<GridConnection>())
{
}

bool PathGridRecord::equals(const PathGridRecord& other) const
{
  return (GridX == other.GridX) && (GridY == other.GridY)
      && (Granularity == other.Granularity) && (CellName == other.CellName)
      && (Points == other.Points) && (Connections == other.Connections);
}

bool PathGridRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cPGRD), 4);
  uint32_t Size = 4 /* DATA */ + 4 /* 4 bytes for length */ + 12 /* size of data */
        + 4 /* NAME */ + 4 /* 4 bytes for length */
        + CellName.length() + 1 /* length of cell name +1 byte for NUL byte */;
  if (!Points.empty())
  {
    Size = Size + 4 /* PGRP */ + 4 /* 4 bytes for length */
         + 16 * Points.size();
  }
  if (!Connections.empty())
  {
    Size = Size + 4 /* PGRC */ + 4 /* 4 bytes for length */
         + 4 * Connections.size();
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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
               uint8_t Flags
               uint8_t ConnectionCount
               int16_t Unknown
             };

    PGRC = unknown data (length: ?)
             Possibly path grid connections?
             Looks like an array of uint32_t pairs, where
             first value is the index of the starting point
             and second value is index of the end point.
  */

  // write path grid data (DATA)
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  uint32_t SubLength = 12;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write path grid data
  output.write(reinterpret_cast<const char*>(&GridX), 4);
  output.write(reinterpret_cast<const char*>(&GridY), 4);
  output.write(reinterpret_cast<const char*>(&Granularity), 2);
  const uint16_t num_of_points = static_cast<uint16_t>(Points.size());
  output.write(reinterpret_cast<const char*>(&num_of_points), 2);

  // write cell name (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  SubLength = CellName.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(CellName.c_str(), SubLength);

  if (!Points.empty())
  {
    // write path grid points (PGRP)
    output.write(reinterpret_cast<const char*>(&cPGRP), 4);
    SubLength = Points.size() * 16; // length is 16 bytes per point
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    // write points
    for (const auto& point: Points)
    {
      // write next point
      output.write(reinterpret_cast<const char*>(&point.X), 4);
      output.write(reinterpret_cast<const char*>(&point.Y), 4);
      output.write(reinterpret_cast<const char*>(&point.Z), 4);
      output.write(reinterpret_cast<const char*>(&point.Flags), 1);
      output.write(reinterpret_cast<const char*>(&point.ConnectionCount), 1);
      output.write(reinterpret_cast<const char*>(&point.Unknown), 2);
    }
  }

  if (!Connections.empty())
  {
    // write path grid connections (PGRC)
    output.write(reinterpret_cast<const char*>(&cPGRC), 4);
    SubLength = Connections.size() * 4; // length is 4 bytes per connection
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    // write connections
    for (const auto& conn: Connections)
    {
      output.write(reinterpret_cast<const char*>(&conn.End), 4);
    }
  }

  return output.good();
}

bool PathGridRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

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
               uint8_t Flags
               uint8_t ConnectionCount
               int16_t Unknown
             };

    PGRC = unknown data (length: ?)
             Possibly path grid connections?
             Looks like an array of uint32_t pairs, where
             first value is the index of the starting point
             and second value is index of the end point.
  */

  uint32_t SubRecName = 0;

  // read DATA
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  uint32_t  BytesRead = 4;
  if (SubRecName != cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  // DATA's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 12)
  {
    std::cerr << "Error: Subrecord DATA of PGRD is has invalid length ("
              << SubLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read path grid's data
  input.read(reinterpret_cast<char*>(&GridX), 4);
  input.read(reinterpret_cast<char*>(&GridY), 4);
  input.read(reinterpret_cast<char*>(&Granularity), 2);
  uint16_t num_of_points = 0;
  input.read(reinterpret_cast<char*>(&num_of_points), 2);
  BytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord DATA of PGRD.\n";
    return false;
  }

  // read cell name (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, CellName, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of PGRD.\n";
    return false;
  }

  Points.clear();
  Connections.clear();
  while (BytesRead < Size)
  {
    // read PGRP or PGRC
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    if (SubRecName == cPGRP)
    {
      // PGRP's length
      input.read(reinterpret_cast<char*>(&SubLength), 4);
      BytesRead += 4;
      if ((SubLength % 16) != 0)
      {
        std::cerr << "Error: Subrecord PGRP of PGRD has invalid length ("
                  << SubLength << " bytes). Should be a multiple of 16 bytes.\n";
        return false;
      }
      // read PGRP
      const unsigned int point_count = SubLength / 16;
      if (point_count != num_of_points)
      {
        std::cerr << "Error: PGRD seems to have " << point_count
                  << " grid points, but according to header there should be "
                  << num_of_points << " points.\n";
        return false;
      }
      GridPointData temp_pt;
      while (Points.size() < point_count)
      {
        // read next point
        input.read(reinterpret_cast<char*>(&temp_pt.X), 4);
        input.read(reinterpret_cast<char*>(&temp_pt.Y), 4);
        input.read(reinterpret_cast<char*>(&temp_pt.Z), 4);
        input.read(reinterpret_cast<char*>(&temp_pt.Flags), 1);
        input.read(reinterpret_cast<char*>(&temp_pt.ConnectionCount), 1);
        input.read(reinterpret_cast<char*>(&temp_pt.Unknown), 2);
        BytesRead += 16;
        if (!input.good())
        {
          std::cerr << "Error while reading subrecord PGRP of PGRD.\n";
          return false;
        }
        Points.push_back(temp_pt);
      }
    } // if PGRP
    else if (SubRecName == cPGRC)
    {
      // Points are required to calculate the start indices.
      if (Points.empty())
      {
        std::cerr << "Error: Subrecord PGRC of PGRD appears without previous PGRP data.\n";
        return false;
      }
      // PGRC's length
      input.read(reinterpret_cast<char*>(&SubLength), 4);
      BytesRead += 4;
      if ((SubLength % 4) != 0)
      {
        std::cerr << "Error: Subrecord PGRC of PGRD has invalid length ("
                  << SubLength << " bytes). Should be a multiple of 4 bytes.\n";
        return false;
      }
      // read PGRC
      const unsigned int connect_count = SubLength / 4;
      GridConnection temp_con;
      while (Connections.size() < connect_count)
      {
        // read next connection index
        // Start indices get adjusted later.
        // Only end indices are part of the sub record's data, so the end
        // indices have to be calculated from the point data.
        temp_con.Start = 0;
        input.read(reinterpret_cast<char*>(&temp_con.End), 4);
        BytesRead += 4;
        if (!input.good())
        {
          std::cerr << "Error while reading subrecord PGRC of PGRD.\n";
          return false;
        }
        Connections.push_back(temp_con);
      }
      if (!calculateStartIndices())
        return false;
    } // if PGRC
    else
    {
      // unexpected record found
      std::cerr << "Error: Expected record name PGRP or PGRC was not found. "
                << "Instead, \"" << IntTo4Char(SubRecName) << "\" was found.\n";
      return false;
    }
  }
  return input.good();
}

bool PathGridRecord::calculateStartIndices()
{
  auto sumConnectionCounts = [](const size_t& sum_so_far, const GridPointData& pt)
  {
    return sum_so_far + pt.ConnectionCount;
  };
  const auto totalConnections = std::accumulate(Points.begin(), Points.end(),
                                  static_cast<size_t>(0), sumConnectionCounts);
  if (totalConnections < Connections.size())
  {
    std::cerr << "Error: PGRD record has not enough grid points to construct all connections!\n";
    return false;
  }
  if (Points.empty())
    return true;
  size_t ptIdx = 0;
  size_t partialSum = Points[0].ConnectionCount;
  const auto connSize = Connections.size();
  for (size_t connIdx = 0; connIdx < connSize; ++connIdx)
  {
    while (connIdx >= partialSum)
    {
      ++ptIdx;
      partialSum += Points.at(ptIdx).ConnectionCount;
    }
    Connections[connIdx].Start = ptIdx;
  }

  return true;
}

} // namespace
