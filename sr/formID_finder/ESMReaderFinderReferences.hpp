/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#ifndef SR_ESMREADERFINDERREFERENCES_HPP
#define SR_ESMREADERFINDERREFERENCES_HPP

#include "../base/ESMReaderReIndex.hpp"
#include <map>
#include <vector>

namespace SRTP
{

/* This descendant of the ESMReader class tries to read all reference records
   from the given .esm/.esp file, that are relevant to the form ID finder.

   Still not complete yet!
*/
class ESMReaderFinderReferences: public ESMReaderReIndexMod
{
  public:
    /* constructor */
    ESMReaderFinderReferences(const std::vector<std::string>& loadOrder);

    /* destructor */
    virtual ~ESMReaderFinderReferences();

    struct CellRefIDPair
    {
      uint32_t cellID;
      uint32_t refID;

      /* constructor */
      CellRefIDPair(const uint32_t cell, const uint32_t ref);
    };//struct

    //fid map - key is base object form ID, value is list of cell IDs
    std::map<uint32_t, std::vector<CellRefIDPair> > refMap;
  protected:
    /* returns true, if the given group may contains some data that the reader
       wants to read. Returns false otherwise.

       parameters:
           g_date - the group header data

       remarks:
           Returns true for all weapon groups, false for any other group.
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

   //cell form ID stack
   std::vector<uint32_t> m_CellStack;
};//class

} //namespace

#endif // SR_ESMREADERFINDERREFERENCES_HPP
