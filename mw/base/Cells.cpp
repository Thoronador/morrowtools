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

#include "Cells.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace MWTP
{

CellRef::CellRef(const CellRecord& rec)
: X(rec.GridX),
  Y(rec.GridY),
  Cell(rec.CellID)
{
}

CellRef::CellRef(const int32_t coordX, const int32_t coordY, const std::string& theCell)
: X(coordX),
  Y(coordY),
  Cell(theCell)
{
}

bool CellRef::operator<(const CellRef& rhs) const
{
  if (X<rhs.X) return true;
  if (X>rhs.X) return false;
  if (Y<rhs.Y) return true;
  if (Y>rhs.Y) return false;
  return (Cell<rhs.Cell);
}

std::string CellRef::toString() const
{
  std::ostringstream sstr;
  sstr<< "PRG(" <<X<<";"<<Y<<"; \""<<Cell<<"\")";
  return sstr.str();
}

Cells::Cells()
: m_Cells(std::map<CellRef, CellRecord>())
{
  //empty
}

Cells::~Cells()
{
  //empty
}

Cells& Cells::getSingleton()
{
  static Cells Instance;
  return Instance;
}

void Cells::addCell(const CellRecord& record)
{
  m_Cells[record] = record;
}

bool Cells::hasCell(const CellRef& ref) const
{
  return (m_Cells.find(ref)!=m_Cells.end());
}

unsigned int Cells::getNumberOfCells() const
{
  return m_Cells.size();
}

const CellRecord& Cells::getCell(const CellRef& ref) const
{
  std::map<CellRef, CellRecord>::const_iterator iter = m_Cells.find(ref);
  if (iter!=m_Cells.end())
  {
    return iter->second;
  }
  std::cout << "No cell with the ref "<<ref.toString()<<" is present.\n";
  throw std::runtime_error("Cells::hasCell(): No cell with the ref "+ref.toString()+" is present.");
}

CellListIterator Cells::getBegin() const
{
  return m_Cells.begin();
}

CellListIterator Cells::getEnd() const
{
  return m_Cells.end();
}

bool Cells::saveAllToStream(std::ostream& output) const
{
  if (!output.good())
  {
    std::cout << "Cells::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  CellListIterator iter = m_Cells.begin();
  const CellListIterator end_iter = m_Cells.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Cells::saveAllToStream: Error while writing record for "
                << iter->first.toString() <<".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Cells::clearAll()
{
  m_Cells.clear();
}

int Cells::readRecordCELL(std::istream& input)
{
  CellRecord temp;
  if(!temp.loadFromStream(input))
  {
    std::cout << "readRecordCELL: Error while reading cell record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasCell(temp))
  {
    if (getCell(temp).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if cell present
  addCell(temp);
  return 1;
} //readRecordCELL

} //namespace
