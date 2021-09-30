/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012  Thoronador

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

#ifndef SR_DATABASE_HPP
#define SR_DATABASE_HPP

#include <map>
#include "records/BasicRecord.hpp"

namespace SRTP
{

  class Database
  {
    public:
      /* destructor */
      virtual ~Database();

      /* singleton access method */
      static Database& get();

      /* Adds a record to the database.

         parameters:
             record - pointer to the record (should not be NULL)

         remarks:
             If a record with the same ID already exists, the old data will be
             replaced by the new data.
             If the record pointer is NULL, no data will be added. This also
             holds true, if the record's ID is an empty string.

             The record pointer must NOT be freed by the application, the
             Database class will take care of this, when necessary.
      */
      void addRecord(BasicRecord* record);

      /* Deletes record with given form ID. Returns true, if such a record was
         present (and thus deleted), false otherwise.

         parameter:
             formID - form ID of the record that should be deleted
      */
      bool deleteRecord(const uint32_t formID);

      /* Returns true, if a record with the given for ID is present, false
         otherwise.

         parameters:
             formID - form ID of the record which will be checked
      */
      bool hasRecord(const uint32_t formID) const;

      /* returns the record with the given ID. If no such record is present, the
         function will throw an exception.

         parameters:
             formID - ID of the requested record
      */
      const BasicRecord& getRecord(const uint32_t formID) const;

      /* Removes all records. */
      void deleteAllRecords();

      /* Returns number of currently available records */
      unsigned int getNumberOfRecords() const;

      //iterator type for iterating through the records
      typedef std::map<uint32_t, BasicRecord*>::const_iterator Iterator;

      /* helper functions to access internal map iterators */
      Iterator getFirst() const;
      Iterator getEnd() const;
    private:
      /* constructor - private due to singleton pattern */
      Database();
      /* copy constructor */
      Database(const Database& op) {}
      std::map<uint32_t, BasicRecord*> m_Records;
  };//class

} //namespace

#endif // SR_DATABASE_HPP
