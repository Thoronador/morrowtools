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

template<typename recT, typename singleT, int32_t headerT>
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
           g_date - the group header data

       remarks:
           Returns true for all keyword groups, false for any other group.
    */
    virtual bool needGroup(const GroupData& g_data) const;

    /* tries to read the next group from a file and returns the number of
       relevant groups that were read (usually one). If an error occured,
       -1 is returned. If the group was skipped or contained no relevant data,
       zero is returned.
       This function is guaranteed to be only called for needed groups (see the
       function needGroup() for details).

       parameters:
           in_File - the input file stream used to read the group
           g_data  - group's data header
    */
    virtual int readGroup(std::ifstream& in_File, const GroupData& g_data);

    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input file stream the record shall be read from
    */
    virtual int readNextRecord(std::ifstream& in_File);
};//class

template<typename recT, typename singleT, int32_t headerT>
int ESMReaderSingleType<recT, singleT, headerT>::readGroup(std::ifstream& in_File, const GroupData& g_data)
{
  //actually read the group
  const std::ifstream::pos_type endPosition = in_File.tellg()+static_cast<std::ifstream::pos_type>(g_data.getGroupSize()-24);
  int recordsRead = 0;
  int lastResult = 0;
  int32_t recName;
  while ((in_File.tellg()<endPosition) and (lastResult>=0))
  {
    //read next header
    recName = 0;
    in_File.read((char*) &recName, 4);
    if (recName!=headerT)
    {
      UnexpectedRecord(headerT, recName);
      return -1;
    }
    lastResult = singleT::getSingleton().readNextRecord(in_File);
    if (lastResult>0)
    {
      recordsRead += lastResult;
    }
  }//while
  if (lastResult>=0)
  {
    if (recordsRead>0) return 1;
    return 0;
  }
  std::cout << "ESMReaderSingleType::readGroup: Error while reading next record!"
            << "\nCurrent position is "<<in_File.tellg()<<" bytes.\n";
  return -1;
}

template<typename recT, typename singleT, int32_t headerT>
bool ESMReaderSingleType<recT, singleT, headerT>::needGroup(const GroupData& g_data) const
{
  return (g_data.getGroupName()==headerT);
}

template<typename recT, typename singleT, int32_t headerT>
int ESMReaderSingleType<recT, singleT, headerT>::readNextRecord(std::ifstream& in_File)
{
  #warning Not implemented yet!
  std::cout << "Error: call to unimplemented function ESMReaderKeywords::readNextRecord!\n";
  return -1;
}

} //namespace

#endif // SR_ESMREADERSINGLETYPE_H
