/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2014  Thoronador

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

#include "PathGrids.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace MWTP
{

PathGridRef::PathGridRef(const PathGridRecord& rec)
: X(rec.GridX),
  Y(rec.GridY),
  Cell(rec.CellName)
{
}

PathGridRef::PathGridRef(const int32_t coordX, const int32_t coordY, const std::string& theCell)
: X(coordX),
  Y(coordY),
  Cell(theCell)
{
}

bool PathGridRef::operator<(const PathGridRef& rhs) const
{
  if (X<rhs.X) return true;
  if (X>rhs.X) return false;
  if (Y<rhs.Y) return true;
  if (Y>rhs.Y) return false;
  return (Cell<rhs.Cell);
}

std::string PathGridRef::toString() const
{
  std::ostringstream sstr("PRG(");
  sstr<<X<<";"<<Y<<"; \""<<Cell<<"\")";
  return sstr.str();
}

PathGrids::PathGrids()
: m_PathGrids(std::map<PathGridRef, PathGridRecord>())
{
  //empty
}

PathGrids::~PathGrids()
{
  //empty
}

PathGrids& PathGrids::getSingleton()
{
  static PathGrids Instance;
  return Instance;
}

void PathGrids::addPathGrid(const PathGridRecord& record)
{
  m_PathGrids[record] = record;
}

bool PathGrids::hasPathGrid(const PathGridRef& ref) const
{
  return (m_PathGrids.find(ref)!=m_PathGrids.end());
}

unsigned int PathGrids::getNumberOfPathGrids() const
{
  return m_PathGrids.size();
}

const PathGridRecord& PathGrids::getPathGrid(const PathGridRef& ref) const
{
  std::map<PathGridRef, PathGridRecord>::const_iterator iter = m_PathGrids.find(ref);
  if (iter!=m_PathGrids.end())
  {
    return iter->second;
  }
  std::cout << "No path grid with the ref "<<ref.toString()<<" is present.\n";
  throw std::runtime_error("No path grid with the ref "+ref.toString()+" is present.");
}

PathGridListIterator PathGrids::getBegin() const
{
  return m_PathGrids.begin();
}

PathGridListIterator PathGrids::getEnd() const
{
  return m_PathGrids.end();
}

bool PathGrids::saveAllToStream(std::ostream& output) const
{
  if (!output.good())
  {
    std::cout << "PathGrids::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  PathGridListIterator iter = m_PathGrids.begin();
  const PathGridListIterator end_iter = m_PathGrids.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "PathGrids::saveAllToStream: Error while writing record for "
                << iter->first.toString() <<".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void PathGrids::clearAll()
{
  m_PathGrids.clear();
}

int PathGrids::readRecordPGRD(std::istream& input)
{
  PathGridRecord temp;
  if(!temp.loadFromStream(input))
  {
    std::cout << "readRecordPGRD: Error while reading path grid record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasPathGrid(temp))
  {
    if (getPathGrid(temp).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if path grid present
  addPathGrid(temp);
  return 1;
} //readRecordPGRD

} //namespace
