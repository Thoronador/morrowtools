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

#ifndef SR_MAPBASEDRECORDMANAGER_H
#define SR_MAPBASEDRECORDMANAGER_H

#include <map>
#include <iostream>
#include <fstream>

namespace SRTP
{

template<typename recT>
class MapBasedRecordManager
{
  public:
    //iterator type for record list/map
    typedef typename std::map<uint32_t, recT>::const_iterator ListIterator;

    /* destructor */
    ~MapBasedRecordManager();

    /* singleton access method */
    static MapBasedRecordManager& getSingleton();

    /* adds a record to the list */
    void addRecord(const recT& record);

    /* returns true, if a record with the given editor ID is present

       parameters:
           ID - the ID of the record object
    */
    bool hasRecord(const uint32_t ID) const;

    /* returns the number of records in the list */
    unsigned int getNumberOfRecords() const;

    /* returns a reference to the record with the given ID

       parameters:
           ID - the ID of the record

       remarks:
           If no record with the given ID is present, the function will throw
           an exception. Use hasRecord() to determine, if a record with the
           desired ID is present.
    */
    const recT& getRecord(const uint32_t ID) const;

    /* tries to read a record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readNextRecord(std::ifstream& in_File);

    /* removes the record with the given ID from the internal list and returns
       true, if such a record existed. Returns false otherwise.

       parameters:
           ID - the ID of the record to be removed
    */
    bool removeRecord(const uint32_t ID);

    /* returns constant iterator to the beginning of the internal list */
    ListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ListIterator getEnd() const;

    /* tries to save all available weapons to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    weapons
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all weapons from the list */
    void clearAll();
  private:
    /* constructor */
    MapBasedRecordManager();

    /* empty copy constructor */
    MapBasedRecordManager(const MapBasedRecordManager& op) {}

    /* internal data */
    std::map<uint32_t, recT> m_Records;
};//class

template<typename recT>
MapBasedRecordManager<recT>::MapBasedRecordManager()
{
  //empty
}

template<typename recT>
MapBasedRecordManager<recT>::~MapBasedRecordManager()
{
  //empty
}

template<typename recT>
MapBasedRecordManager<recT>& MapBasedRecordManager<recT>::getSingleton()
{
  static MapBasedRecordManager<recT> Instance;
  return Instance;
}

template<typename recT>
void MapBasedRecordManager<recT>::addRecord(const recT& record)
{
  if (record.headerFormID!=0)
  {
    m_Records[record.headerFormID] = record;
  }
}

template<typename recT>
bool MapBasedRecordManager<recT>::hasRecord(const uint32_t ID) const
{
  return (m_Records.find(ID)!=m_Records.end());
}

template<typename recT>
unsigned int MapBasedRecordManager<recT>::getNumberOfRecords() const
{
  return m_Records.size();
}

template<typename recT>
const recT& MapBasedRecordManager<recT>::getRecord(const uint32_t ID) const
{
  ListIterator iter = m_Records.find(ID);
  if (iter!=m_Records.end())
  {
    return iter->second;
  }
  std::cout << "MapBasedRecordManager: Error! No record with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

template<typename recT>
bool MapBasedRecordManager<recT>::removeRecord(const uint32_t ID)
{
  return (m_Records.erase(ID)!=0);
}

template<typename recT>
typename MapBasedRecordManager<recT>::ListIterator
MapBasedRecordManager<recT>::getBegin() const
{
  return m_Records.begin();
}

template<typename recT>
typename MapBasedRecordManager<recT>::ListIterator MapBasedRecordManager<recT>::getEnd() const
{
  return m_Records.end();
}

template<typename recT>
bool MapBasedRecordManager<recT>::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "MapBasedRecordManager::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  ListIterator iter = m_Records.begin();
  const ListIterator end_iter = m_Records.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "MapBasedRecordManager::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

template<typename recT>
void MapBasedRecordManager<recT>::clearAll()
{
  m_Records.clear();
}

template<typename recT>
int MapBasedRecordManager<recT>::readNextRecord(std::ifstream& in_File)
{
  recT temp;
  if(!temp.loadFromStream(in_File))
  {
    std::cout << "MapBasedRecordManager::readNextRecord: Error while reading record.\n";
    return -1;
  }

  //add it to the list, if not present with same data
  if (hasRecord(temp.headerFormID))
  {
    if (getRecord(temp.headerFormID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if record present
  addRecord(temp);
  return 1;
} //readOneRecord

} //namespace

#endif // SR_MAPBASEDRECORDMANAGER_H
