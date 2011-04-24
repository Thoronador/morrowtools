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

#ifndef ESMREADERTRANSLATOR_H
#define ESMREADERTRANSLATOR_H

#include "../base/ESMReaderGeneric.h"

class ESMReaderTranslator: public ESMReaderGeneric
{
  public:
    /* constructor

       parameters:
           vec - pointer to the vector that will hold the records that will be
                 read. This pointer must not be NULL.
    */
    ESMReaderTranslator(VectorType* vec);

    /* destructor */
    virtual ~ESMReaderTranslator();
  protected:
    /* tries to read the next record from a file and returns the number of
       relevant records that were read (usually one). If an error occured,
       -1 is returned. If the record was skipped or contained no relevant data,
       zero is returned.

       parameters:
           in_File  - the input file stream the record shall be read from
    */
    virtual int processNextRecord(std::ifstream& in_File);
};//class

#endif // ESMREADERTRANSLATOR_H
