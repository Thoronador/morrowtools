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

#ifndef ESMREADERSCRIPTCOMPILER_H
#define ESMREADERSCRIPTCOMPILER_H

#include "ESMReader.h"

namespace MWTP
{

/* This descendant of the ESMReader class tries to read all records from the
   given .esm/.esp file, which would be required to get enough data for properly
   operating the script "compiler".
*/
class ESMReaderScriptCompiler: public ESMReader
{
  public:
    /* constructor */
    ESMReaderScriptCompiler();

    /* destructor */
    virtual ~ESMReaderScriptCompiler();
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

} //namespace

#endif // ESMREADERSCRIPTCOMPILER_H
