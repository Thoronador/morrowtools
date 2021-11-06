/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef MW_ESMREADERGENERIC_HPP
#define MW_ESMREADERGENERIC_HPP

#include <vector>
#include "ESMReader.hpp"
#include "records/BasicRecord.hpp"

namespace MWTP
{

/* This descendant of the ESMReader class tries to read all records from the
   given .esm/.esp file and uses the supplied std::vector of GenericRecord
   structures (hence the name) to store the read data records.
*/
class ESMReaderGeneric: public ESMReader
{
  public:
    //shortcut for the type of the used vector
    typedef std::vector<BasicRecord*> VectorType;

    /* constructor

       parameters:
           vec - pointer to the vector that will hold the records that will be
                 read. This pointer must not be NULL and has to live as long as
                 the ESMReaderGeneric, at least.
    */
    ESMReaderGeneric(VectorType* vec);

    /* destructor */
    virtual ~ESMReaderGeneric();

    /// delete copy constructor
    ESMReaderGeneric(const ESMReaderGeneric& other) = delete;
    /// assignment operator
    ESMReaderGeneric& operator=(const ESMReaderGeneric& other) = delete;

    /* tries to remove all record pointers from the given vector and deallocates
       them in turn
    */
    void deallocateRecordsInVector();
  protected:
    /* tries to read the next record from a stream and returns the number of
       relevant records that were read (usually one). If an error occurred,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input file stream the record shall be read from
    */
    virtual int processNextRecord(std::istream& in_File) override;

    /* pointer to the vector where the read data will be stored*/
    VectorType* m_VectorPointer;
};//class

} //namespace

#endif // MW_ESMREADERGENERIC_HPP
