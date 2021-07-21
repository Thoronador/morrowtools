/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012, 2013, 2014  Thoronador

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

#ifndef MW_MAPBASEDRECORDMANAGER_HPP
#define MW_MAPBASEDRECORDMANAGER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include "../../base/ComparisonFunctor.hpp"

namespace MWTP
{

template<typename recT>
class MapBasedRecordManager
{
  public:
    //iterator type for record list/map
    typedef typename std::map<std::string, recT, ci_less>::const_iterator ListIterator;

    /* destructor */
    ~MapBasedRecordManager();

    /* singleton access method */
    static MapBasedRecordManager& getSingleton();

    /* adds a record to the list */
    void addRecord(const recT& record);

    /* returns true, if a record with the given ID is present

       parameters:
           ID - the ID of the record
    */
    bool hasRecord(const std::string& ID) const;

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
    const recT& getRecord(const std::string& ID) const;

    /* tries to read a record from the given input file stream.

       return value:
           If an error occurred, the function returns -1. Otherwise it returns
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
    bool removeRecord(const std::string& ID);

    /* returns constant iterator to the beginning of the internal list */
    ListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    ListIterator getEnd() const;

    #ifndef MW_UNSAVEABLE_RECORDS
    /* tries to save all available records to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    records
    */
    bool saveAllToStream(std::ofstream& output) const;
    #endif

    /* removes all records from the list */
    void clearAll();
  private:
    /* constructor */
    MapBasedRecordManager();

    /* empty copy constructor */
    MapBasedRecordManager(const MapBasedRecordManager& op) {}

    /* internal data */
    std::map<std::string, recT, ci_less> m_Records;
};//class

template<typename recT>
MapBasedRecordManager<recT>::MapBasedRecordManager()
: m_Records(std::map<std::string, recT, ci_less>())
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
  if (!record.recordID.empty())
  {
    m_Records[record.recordID] = record;
  }
}

template<typename recT>
bool MapBasedRecordManager<recT>::hasRecord(const std::string& ID) const
{
  return (m_Records.find(ID)!=m_Records.end());
}

template<typename recT>
unsigned int MapBasedRecordManager<recT>::getNumberOfRecords() const
{
  return m_Records.size();
}

template<typename recT>
const recT& MapBasedRecordManager<recT>::getRecord(const std::string& ID) const
{
  const ListIterator iter = m_Records.find(ID);
  if (iter!=m_Records.end())
  {
    return iter->second;
  }
  std::cout << "MapBasedRecordManager: Error! No record with the ID \""<<ID<<"\" is present.\n";
  throw std::runtime_error("MapBasedRecordManager: Error! No record with the ID \""+ID+"\" is present.");
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

  #if !defined(MW_NO_SINGLETON_EQUALITY_CHECK) && !defined(MW_NO_RECORD_EQUALITY)
  //add it to the list, if not present with same data
  if (hasRecord(temp.recordID))
  {
    if (getRecord(temp.recordID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if record present
  #endif //MW_NO_SINGLETON_EQUALITY_CHECK
  addRecord(temp);
  return 1;
}

template<typename recT>
bool MapBasedRecordManager<recT>::removeRecord(const std::string& ID)
{
  return (m_Records.erase(ID)!=0);
}

template<typename recT>
typename MapBasedRecordManager<recT>::ListIterator MapBasedRecordManager<recT>::getBegin() const
{
  return m_Records.begin();
}

template<typename recT>
typename MapBasedRecordManager<recT>::ListIterator MapBasedRecordManager<recT>::getEnd() const
{
  return m_Records.end();
}

#ifndef MW_UNSAVEABLE_RECORDS
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
#endif

template<typename recT>
void MapBasedRecordManager<recT>::clearAll()
{
  m_Records.clear();
}

} //namespace

#endif // MW_MAPBASEDRECORDMANAGER_HPP
