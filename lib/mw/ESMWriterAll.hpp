/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2024  Dirk Stolle

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

#ifndef MW_ESMWRITERALL_HPP
#define MW_ESMWRITERALL_HPP

#include "ESMWriter.hpp"

namespace MWTP
{

class ESMWriterAll: public ESMWriter
{
  public:
    ESMWriterAll();
  protected:
    /* returns the number of records that will be written to the stream */
    virtual int32_t getTotalRecords() const override;

    /* tries to write all data records and returns true on success

       parameters:
           output - the output stream that's used to write the records
    */
    virtual bool writeRecords(std::ostream& output) const override;
}; //class

} //namespace

#endif // MW_ESMWRITERALL_HPP
