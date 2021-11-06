/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef MW_BOOKRECORD_HPP
#define MW_BOOKRECORD_HPP

#include <string>
#include <fstream>
#include "BasicRecord.hpp"

namespace MWTP
{

struct BookRecord: public BasicRecord
{
  public:
    std::string recordID; //formerly BookID
    std::string MeshPath;
    std::string Name;
    //book data
    float Weight;
    int32_t Value;
    int32_t Scroll;
    int32_t SkillID;
    int32_t EnchantPoints;
    //end of book data
    std::string InventoryIcon;
    std::string ScriptID;
    std::string Text;
    std::string EnchantmentID;

    /* constructor */
    BookRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const BookRecord& other) const;

    #ifndef MW_UNSAVEABLE_RECORDS
    /* writes the record to the given output stream and returns true on success

      parameters:
          output - the output stream
    */
    bool saveToStream(std::ostream& output) const override;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input stream
    */
    bool loadFromStream(std::istream& in_File) override;
};//struct

} //namespace

#endif // MW_BOOKRECORD_HPP
