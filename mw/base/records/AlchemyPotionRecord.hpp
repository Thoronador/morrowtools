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

#ifndef MW_ALCHEMYPOTIONRECORD_HPP
#define MW_ALCHEMYPOTIONRECORD_HPP

#include <string>
#include <fstream>
#include <vector>
#include "BasicRecord.hpp"
#include "../Enchantment.hpp"

namespace MWTP
{

struct AlchemyPotionRecord: public BasicRecord
{
  public:
    std::string recordID; //formerly known as AlchemyID
    std::string ModelPath;
    std::string Name;
    //alchemy data
    float Weight;
    int32_t Value;
    int32_t AutoCalc;
    //end alchemy data
    std::vector<EnchantmentData> Enchs;
    std::string InventoryIcon;
    std::string ScriptName;

    /* constructor */
    AlchemyPotionRecord();

    /* returns true, of content of other record is equal to this one */
    bool equals(const AlchemyPotionRecord& other) const;

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
  protected:
    /*tries to read a subrecord for the alchemy data and returns true on success

      parameters:
          in_File   - the input stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordALDT(std::istream& in_File, char* Buffer, uint32_t& BytesRead);

    /*tries to read a subrecord for the enchantment and returns true on success

      parameters:
          in_File   - the input stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordENAM(std::istream& in_File, char* Buffer, uint32_t& BytesRead);

    /*tries to read a subrecord for the item name and returns true on success

      parameters:
          in_File   - the input stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordFNAM(std::istream& in_File, char* Buffer, uint32_t& BytesRead);

    /*tries to read a subrecord for the script ID and returns true on success

      parameters:
          in_File   - the input stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordSCRI(std::istream& in_File, char* Buffer, uint32_t& BytesRead);

    /*tries to read a subrecord for the inventory icon and returns true on
      success

      parameters:
          in_File   - the input stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordTEXT(std::istream& in_File, char* Buffer, uint32_t& BytesRead);
}; //struct

} //namespace

#endif // MW_ALCHEMYPOTIONRECORD_HPP
