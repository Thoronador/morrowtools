/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef MW_ESMWRITERSKILLREBALANCE_H
#define MW_ESMWRITERSKILLREBALANCE_H

#include "../base/ESMWriter.h"

namespace MWTP
{

class ESMWriterSkillRebalance: public ESMWriter
{
  public:
    /* constructor */
    ESMWriterSkillRebalance();

    /* destructor */
    virtual ~ESMWriterSkillRebalance();
  protected:
    /* returns the number of records that will be written to the stream */
    virtual int32_t getTotalRecords() const;

    /* tries to write all data records and returns true on success

       parameters:
           output - the output file stream that's used to write the records
    */
    virtual bool writeRecords(std::ofstream& output) const;
}; //class

} //namespace

#endif // MW_ESMWRITERSKILLREBALANCE_H
