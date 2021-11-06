/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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

#ifndef MW_ESMREADERSKILLREBALANCE_HPP
#define MW_ESMREADERSKILLREBALANCE_HPP

#include <vector>
#include "../base/ESMReaderGeneric.hpp"
#include "../base/records/BasicRecord.hpp"

namespace MWTP
{

/* This descendant of the ESMReader class tries to read all skill records from
   the given .esm/.esp file.
*/
class ESMReaderSkillRebalance: public ESMReader
{
  public:
    /* constructor */
    ESMReaderSkillRebalance();

    /* destructor */
    virtual ~ESMReaderSkillRebalance();
  protected:
    /* tries to read the next record from a stream and returns the number of
       relevant records that were read (usually one). If an error occurred,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File - the input stream the record shall be read from
    */
    virtual int processNextRecord(std::istream& in_File) override;
};//class

} //namespace

#endif // MW_ESMREADERSKILLREBALANCE_HPP
