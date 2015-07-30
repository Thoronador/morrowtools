/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_ESMWRITERSINGLETYPE_H
#define SR_ESMWRITERSINGLETYPE_H

#include <iostream>
#include "ESMWriter.h"
#include "records/GroupData.h"

#ifndef SR_UNSAVEABLE_RECORDS

//stuff goes here
namespace SRTP
{

/* This descendant of the ESMWriter class tries to write all records of a given
   type, hold in a MapBasedRecordManager class, to the given .esm/.esp file.
   This class is layed out for writing top level groups which contain only one
   distinct record type and have no subgroups. It will only write one single
   top level group (or no group at all, if the singleton contains no records) to
   the given file.

   If you want/need to write a file structure that contains nested groups or
   more than one group, try the ESMWriterContents class instead.
*/
template<typename recT, typename singleT>
class ESMWriterSingleType: public ESMWriter
{
  public:
    /* constructor */
    ESMWriterSingleType()
    {
      //empty
    }

    /* destructor */
    virtual ~ESMWriterSingleType()
    {
      //empty
    }
  protected:
    /* returns the number of records that will be written to the stream, not
       including the header record

       remarks:
           This function has to be implemented by every derived class to match
           the actual number of records.
    */
    virtual uint32_t getTotalNumberOfRecords() const;

    /* returns the number of groups that will be written to the stream

       remarks:
           This function has to be implemented by every derived class to match
           the actual number of groups.
    */
    virtual uint32_t getTotalNumberOfGroups() const;

    /* tries to write all gropus and data records and returns true on success

       parameters:
           output - the output file stream that's used to write the data
    */
    virtual bool writeGroups(std::ofstream& output) const;
};//class

template<typename recT, typename singleT>
uint32_t ESMWriterSingleType<recT, singleT>::getTotalNumberOfRecords() const
{
  return singleT::getSingleton().getNumberOfRecords();
}

template<typename recT, typename singleT>
uint32_t ESMWriterSingleType<recT, singleT>::getTotalNumberOfGroups() const
{
  if (singleT::getSingleton().getNumberOfRecords()>0) return 1;
  //no records, thus no group to write
  return 0;
}

template<typename recT, typename singleT>
bool ESMWriterSingleType<recT, singleT>::writeGroups(std::ofstream& output) const
{
  //prepare group header
  GroupData headerData;
  recT dummyRec;
  headerData.setGroupLabel(dummyRec.getRecordType());
  headerData.setGroupType(GroupData::cTopLevelGroup);
  // -- calculate size
  uint32_t total = 0;
  // -- ... by adding up record sizes
  typename singleT::ListIterator iter = singleT::getSingleton().getBegin();
  while (iter!=singleT::getSingleton().getEnd())
  {
    total += iter->second.getTotalWrittenSize();
    ++iter;
  }//while
  headerData.setGroupSize(total);

  //now write the actual stuff
  // ---- group header goes first
  if (!headerData.saveToStream(output))
  {
    std::cout << "ESMWriterSingleType::writeGroups: Error: could not write group header!\n";
    return false;
  }
  // ---- records come next
  return singleT::getSingleton().saveAllToStream(output);
}

} //namespace

#endif // SR_UNSAVEABLE_RECORDS

#endif // SR_ESMWRITERSINGLETYPE_H
