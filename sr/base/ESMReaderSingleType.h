/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_ESMREADERSINGLETYPE_H
#define SR_ESMREADERSINGLETYPE_H

#include "ESMReader.h"
#include <stdint.h>
#include <iostream>
#include "../../mw/base/HelperIO.h"

namespace SRTP
{

/* This descendant of the ESMReader class tries to read all records of a given
   type from the given .esm/.esp file.
*/

template<typename recT, typename singleT, uint32_t headerT>
class ESMReaderSingleType: public ESMReader
{
  public:
    /* constructor */
    ESMReaderSingleType()
    {
      //empty
    }

    /* destructor */
    virtual ~ESMReaderSingleType()
    {
      //empty
    }
  protected:
    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_data - the group header data

       remarks:
           Returns true for all top level groups of given type, false for any
           other group.
    */
    virtual bool needGroup(const GroupData& g_data) const;

    /* This function's sole purpose is to "notify" the reader that a new group
       was encountered and give the classes derived from ESMReader the
       possibility to update their internal state accordingly. Note that this
       function will only be called for groups where needGroup() returns true.
       Skipped groups will not trigger that function.
       Shall return false only if the read process should be aborted.

       parameters:
           g_data - the group header data
           sub    - if set to true, the new group is a sub-group of another
                    group
    */
    virtual bool nextGroupStarted(const GroupData& g_data, const bool sub);

    /* This function's sole purpose is to "notify" the reader that a started
       group has been read (or skipped) completely and give the classes derived
       from ESMReader the possibility to update their internal state
       accordingly. Note that this function will only be called for groups
       where needGroup() returns true. Skipped groups will not trigger that
       function, but it might be possible that all records and subgroups in such
       a group have been skipped, depending on the implementation of
       readNextRecord() and readGroup().
       Shall return false only if the read process should be aborted.

       parameters:
           g_data - the group header data
    */
    virtual bool groupFinished(const GroupData& g_data);

    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occurred,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File   - the input file stream the record shall be read from
           recName   - name (header) of the next record
           localized - true, if the data in the stream is localized
           table     - in case of localized data: the string table
    */
    virtual int readNextRecord(std::ifstream& in_File, const uint32_t recName, const bool localized, const StringTable& table);
};//class

template<typename recT, typename singleT, uint32_t headerT>
bool ESMReaderSingleType<recT, singleT, headerT>::needGroup(const GroupData& g_data) const
{
  return ((g_data.getGroupLabel()==headerT) and (g_data.getGroupType()==GroupData::cTopLevelGroup));
}

template<typename recT, typename singleT, uint32_t headerT>
bool ESMReaderSingleType<recT, singleT, headerT>::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  //empty, because we don't need to care about new groups anyway
  return true;
}

template<typename recT, typename singleT, uint32_t headerT>
bool ESMReaderSingleType<recT, singleT, headerT>::groupFinished(const GroupData& g_data)
{
  //empty, because we don't need to care about new groups anyway
  return true;
}

template<typename recT, typename singleT, uint32_t headerT>
int ESMReaderSingleType<recT, singleT, headerT>::readNextRecord(std::ifstream& in_File, const uint32_t recName, const bool localized, const StringTable& table)
{
  if (recName!=headerT)
  {
    UnexpectedRecord(headerT, recName);
    return -1;
  }
  return singleT::getSingleton().readNextRecord(in_File, localized, table);
}

} //namespace

#endif // SR_ESMREADERSINGLETYPE_H
