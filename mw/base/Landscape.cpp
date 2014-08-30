/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2014 Thoronador

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

#include "Landscape.h"
#include <iostream>
#include <stdexcept>

namespace MWTP
{

LandscapeCoords::LandscapeCoords(const LandscapeRecord& rec)
: X(rec.CellX),
  Y(rec.CellY)
{
}

LandscapeCoords::LandscapeCoords(const int32_t cellX, const int32_t cellY)
: X(cellX),
  Y(cellY)
{
}

bool LandscapeCoords::operator<(const LandscapeCoords& rhs) const
{
  if (X<rhs.X) return true;
  if (X>rhs.X) return false;
  return (Y<rhs.Y);
}

Landscape::Landscape()
: m_Landscape(std::map<LandscapeCoords, LandscapeRecord*>())
{
  //empty
}

Landscape::~Landscape()
{
  clearAll();
}

Landscape& Landscape::getSingleton()
{
  static Landscape Instance;
  return Instance;
}

void Landscape::addLandscapeRecord(LandscapeRecord* record)
{
  if (record!=NULL)
  {
    std::map<LandscapeCoords, LandscapeRecord*>::iterator iter = m_Landscape.find(*record);
    if (iter!=m_Landscape.end())
    {
      //record with same coordinates is already present - delete it first
      delete iter->second;
      //iter->second = NULL;
      iter->second = record;
      return;
    }
    //assign new record
    m_Landscape[*record] = record;
  }//if record is not NULL
}

bool Landscape::hasLandscapeRecord(const LandscapeCoords& coords) const
{
  return (m_Landscape.find(coords)!=m_Landscape.end());
}

unsigned int Landscape::getNumberOfLandscapeRecords() const
{
  return m_Landscape.size();
}

const LandscapeRecord& Landscape::getLandscapeRecord(const LandscapeCoords& coords) const
{
  LandscapeListIterator iter = m_Landscape.find(coords);
  if (iter!=m_Landscape.end())
  {
    return *(iter->second);
  }
  std::cout << "No landscape record with the coordinates ("<<coords.X<<";"
            << coords.Y<<") is present.\n";
  throw std::runtime_error("No landscape record with the given coordinates is present.");
}

LandscapeListIterator Landscape::getBegin() const
{
  return m_Landscape.begin();
}

LandscapeListIterator Landscape::getEnd() const
{
  return m_Landscape.end();
}

bool Landscape::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Landscape::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  LandscapeListIterator iter = m_Landscape.begin();
  const LandscapeListIterator end_iter = m_Landscape.end();
  while (iter!=end_iter)
  {
    if (!(iter->second->saveToStream(output)))
    {
      std::cout << "Landscape::saveAllToStream: Error while writing record for "
                << "cell ("<<iter->first.X<<";"<<iter->first.Y<<").\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Landscape::clearAll()
{
  std::map<LandscapeCoords, LandscapeRecord*>::iterator iter = m_Landscape.begin();
  while (iter!=m_Landscape.end())
  {
    delete (iter->second);
    iter->second = NULL;
    ++iter;
  }//while
  //clear all pointers from list - should be NULL by now anyway
  m_Landscape.clear();
}

int Landscape::readRecordLAND(std::ifstream& in_File)
{
  LandscapeRecord* temp;
  temp = new LandscapeRecord;
  if(!temp->loadFromStream(in_File))
  {
    std::cout << "Landscape::readRecordLAND: Error while reading landscape record.\n";
    delete temp;
    temp = NULL;
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasLandscapeRecord(*temp))
  {
    if (getLandscapeRecord(*temp).equals(*temp))
    {
      //same record with equal data is already present, return zero
      delete temp;
      temp = NULL;
      return 0;
    }
  }//if landscape record present
  addLandscapeRecord(temp);
  return 1;
} //readRecordLAND

} //namespace
