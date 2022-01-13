/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2022  Dirk Stolle

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

/** Represents a point in the path grid. */
struct GridPointData
{
  int32_t X;
  int32_t Y;
  int32_t Z;
  uint8_t Flags;
  uint8_t ConnectionCount;
  uint16_t Unknown;

  GridPointData();

  bool operator==(const GridPointData& other) const;
}; // struct

/** Represents a connection between two grid points, i. e. an edge. */
struct GridConnection
{
  uint32_t Start; /**< zero-based index of the point where the edge starts */
  uint32_t End;   /**< zero-based index of the point where the edge ends */

  GridConnection();

  bool operator==(const GridConnection& other) const;
}; // struct

struct PathGridRecord: public BasicRecord
{
  // path grid data
  int32_t GridX;
  int32_t GridY;
  uint16_t Granularity;
  // end of path grid data
  std::string CellName;
  std::vector<GridPointData> Points;
  std::vector<GridConnection> Connections;

  PathGridRecord();

  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as instance.
   *         Returns false otherwise.
   */
  bool equals(const PathGridRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Writes the record to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (record was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const override;
  #endif

  /** \brief Loads the record from the given input stream.
   *
   * \param input    the input stream
   * \return Returns true on success (record was loaded from stream).
   *         Returns false, if an error occurred.
   */
  bool loadFromStream(std::istream& input) override;
private:
  /** \brief Calculates the start indices of connections.
   *
   * \return Returns true on success.
   *         Returns false on failure.
   * \remarks This should not be called manually.
   */
  bool calculateStartIndices();
}; // struct

} // namespace

#endif // MW_PATHGRIDRECORD_HPP
