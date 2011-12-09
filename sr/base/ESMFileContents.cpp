/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "ESMFileContents.h"

namespace SRTP
{

ESMFileContents::ESMFileContents()
{
  m_Records.clear();
  m_Index.clear();
}

ESMFileContents::~ESMFileContents()
{
  removeContents();
}

void ESMFileContents::addRecord(BasicRecord* rec)
{
  if (rec!=NULL)
  {
    m_Index[rec->headerFormID] = m_Records.size();
    m_Records.push_back(rec);
  }//if
}

bool ESMFileContents::hasRecord(const uint32_t formID, const bool useIndex) const
{
  if (useIndex) return (m_Index.find(formID)!=m_Index.end());
  unsigned int i;
  for (i=0; i<m_Records.size(); ++i)
  {
    if (m_Records[i]->headerFormID==formID) return true;
  }//for
  return false;
}

const BasicRecord& ESMFileContents::getRecord(const uint32_t formID, const bool useIndex) const
{
  if (useIndex)
  {
    const std::map<uint32_t, uint32_t>::const_iterator iter = m_Index.find(formID);
    if (iter!=m_Index.end())
    {
      if (iter->second<m_Records.size()) return *m_Records[iter->second];
    }
  }//if index
  else
  {
    unsigned int i;
    for (i=0; i<m_Records.size(); ++i)
    {
      if (m_Records[i]->headerFormID==formID) return *m_Records[i];
    }//for
  }//else
  //record was not found
  throw "Record with requested form ID not found!\n";
}

unsigned int ESMFileContents::getNumberOfRecords() const
{
  return m_Records.size();
}

void ESMFileContents::rebuildIndex()
{
  m_Index.clear();
  uint32_t i;
  for (i=0; i<m_Records.size(); ++i)
  {
    m_Index[m_Records[i]->headerFormID] = i;
  }//for
}

void ESMFileContents::removeContents()
{
  m_Index.clear();
  BasicRecord * recPtr;
  while (!m_Records.empty())
  {
    recPtr = m_Records.back();
    m_Records.pop_back();
    delete recPtr;
  }//while
}

} //namespace