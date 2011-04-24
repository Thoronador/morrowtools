/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef ESMWRITERGENERIC_H
#define ESMWRITERGENERIC_H

#include "ESMWriter.h"
#include "records/BasicRecord.h"

/* This descendant of the ESMWriter class tries to write all records in the
   supplied std::vector of BasicRecord structures to a file.
*/
class ESMWriterGeneric: public ESMWriter
{
  public:
    //shortcut for the type of the used vector
    typedef std::vector<BasicRecord*> VectorType;

    /* constructor

       parameters:
           vec - pointer to the vector that will hold the records that will be
                 written. This pointer must not be NULL and has to live as long as
                 the ESMWriterGeneric, at least.
    */
    ESMWriterGeneric(VectorType* vec);

    /* destructor */
    virtual ~ESMWriterGeneric();
  protected:
    /* returns the number of records that will be written to the stream */
    virtual int32_t getTotalRecords() const;

    /* tries to write all data records and returns true on success

       parameters:
           output - the output file stream that's used to write the records
    */
    virtual bool writeRecords(std::ofstream& output) const;

    /* pointer to the vector where the read data will be stored*/
    VectorType* m_VectorPointer;
}; //class

#endif // ESMWRITERGENERIC_H
