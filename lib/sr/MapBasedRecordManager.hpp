/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014, 2021, 2023  Dirk Stolle

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

#ifndef SR_MAPBASEDRECORDMANAGER_HPP
#define SR_MAPBASEDRECORDMANAGER_HPP

#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include "StringTable.hpp"

namespace SRTP
{

/** Singleton-based manager for records of a given type recT, using std::map. */
template<typename recT>
class MapBasedRecordManager
{
  public:
    /// iterator type for internal map structure
    using ListIterator = typename std::map<uint32_t, recT>::const_iterator;

    /** \brief Provides access to the singleton instance.
     *
     * \return Returns a reference to the singleton instance.
     */
    static MapBasedRecordManager& get();

    /** \brief Adds a record to the instance.
     *
     * \param record   the record to add
     * \remarks The record will NOT be added, if its form ID is zero.
     *          An existing record with the same form ID will be replaced.
     */
    void addRecord(const recT& record);

    /** \brief Checks whether a record with the given form ID is present.
     *
     * \param ID  the form ID of the record object
     * \return Returns true, if a record with the given form ID is present.
     *         Returns false otherwise.
     */
    bool hasRecord(const uint32_t ID) const;

    /** Gets the number of records in the instance.
     *
     * \return Returns the number of records in the instance.
     */
    unsigned int getNumberOfRecords() const;

    /** Gets a reference to the record with the given form ID.
     *
     * \param ID  the form ID of the record
     * \return Returns a reference to the record with the given ID, if such a
     *         record is present. Throws, if no such record exists.
     * \remarks If no record with the given ID is present, the function will
     *          throw an exception. Use hasRecord() to determine, if a record
     *          with the desired ID is present.
     */
    const recT& getRecord(const uint32_t ID) const;

    /** \brief Tries to read a record from the given input stream.
     *
     * \param input       the input stream that is used to read the record
     * \param localized   true, if the data in the stream is localized
     * \param table       in case of localized data: the string table
     * \return If an error occurred, the function returns -1. Otherwise it
     *         returns the number of updated records. (Usually that is one.
     *         If, however, the record that was read is equal to the one already
     *         in the list, zero is returned and the existing record remains
     *         unchanged.)
     */
    int readNextRecord(std::istream& input, const bool localized, const StringTable& table);

    /** \brief Removes the record with the given ID from the instance.
     *
     * \param ID  the form ID of the record to be removed
     * \return Returns true, if a record was removed. Returns false otherwise.
     */
    bool removeRecord(const uint32_t ID);

    /** Returns constant iterator to the beginning of the internal structure. */
    ListIterator begin() const noexcept;

    /** Returns constant iterator to the end of the internal structure. */
    ListIterator end() const noexcept;

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Tries to save all available records to the given stream.
     *
     * \param output  the output stream that shall be used to save the records
     * \return Returns true on success, false on failure.
     */
    bool saveAllToStream(std::ostream& output) const;
    #endif

    /** Removes all records from the instance. */
    void clear();
  private:
    /** Constructor. */
    MapBasedRecordManager();

    /** Deleted copy constructor. */
    MapBasedRecordManager(const MapBasedRecordManager& op) = delete;

    /** Deleted move constructor. */
    MapBasedRecordManager(MapBasedRecordManager&& op) = delete;

    std::map<uint32_t, recT> m_Records; /**< internal data */
};

template<typename recT>
MapBasedRecordManager<recT>::MapBasedRecordManager()
: m_Records(std::map<uint32_t, recT>())
{
}

template<typename recT>
MapBasedRecordManager<recT>& MapBasedRecordManager<recT>::get()
{
  static MapBasedRecordManager<recT> Instance;
  return Instance;
}

template<typename recT>
void MapBasedRecordManager<recT>::addRecord(const recT& record)
{
  if (record.headerFormID != 0)
  {
    m_Records[record.headerFormID] = record;
  }
}

template<typename recT>
bool MapBasedRecordManager<recT>::hasRecord(const uint32_t ID) const
{
  return m_Records.find(ID) != m_Records.end();
}

template<typename recT>
unsigned int MapBasedRecordManager<recT>::getNumberOfRecords() const
{
  return m_Records.size();
}

template<typename recT>
const recT& MapBasedRecordManager<recT>::getRecord(const uint32_t ID) const
{
  const ListIterator iter = m_Records.find(ID);
  if (iter != m_Records.end())
  {
    return iter->second;
  }
  std::cerr << "MapBasedRecordManager: Error! No record with the ID \"" << ID
            << "\" is present.\n";
  throw std::runtime_error("MapBasedRecordManager: Error! No record with the requested ID is present.");
}

template<typename recT>
bool MapBasedRecordManager<recT>::removeRecord(const uint32_t ID)
{
  return m_Records.erase(ID) !=0;
}

template<typename recT>
typename MapBasedRecordManager<recT>::ListIterator
MapBasedRecordManager<recT>::begin() const noexcept
{
  return m_Records.begin();
}

template<typename recT>
typename MapBasedRecordManager<recT>::ListIterator MapBasedRecordManager<recT>::end() const noexcept
{
  return m_Records.end();
}

#ifndef SR_UNSAVEABLE_RECORDS
template<typename recT>
bool MapBasedRecordManager<recT>::saveAllToStream(std::ostream& output) const
{
  if (!output.good())
  {
    std::cerr << "MapBasedRecordManager::saveAllToStream: Error: Bad stream.\n";
    return false;
  }
  for (const auto& [id, record]: m_Records)
  {
    if (!record.saveToStream(output))
    {
      std::cerr << "MapBasedRecordManager::saveAllToStream: Error while writing record for \""
                << record.headerFormID << "\".\n";
      return false;
    }
  }
  return output.good();
}
#endif

template<typename recT>
void MapBasedRecordManager<recT>::clear()
{
  m_Records.clear();
}

template<typename recT>
int MapBasedRecordManager<recT>::readNextRecord(std::istream& input, const bool localized, const StringTable& table)
{
  recT temp;
  if(!temp.loadFromStream(input, localized, table))
  {
    std::cerr << "MapBasedRecordManager::readNextRecord: Error while reading record.\n";
    return -1;
  }

  #if !defined(SR_NO_SINGLETON_EQUALITY_CHECK) && !defined(SR_NO_RECORD_EQUALITY)
  // add it to the list, if not present with same data
  if (hasRecord(temp.headerFormID))
  {
    if (getRecord(temp.headerFormID).equals(temp))
    {
      // Same record with equal data is already present, return zero.
      return 0;
    }
  }
  #endif // SR_NO_SINGLETON_EQUALITY_CHECK
  addRecord(temp);
  return 1;
}

} // namespace

#endif // SR_MAPBASEDRECORDMANAGER_HPP
