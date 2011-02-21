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

#ifndef ALCHEMYPOTIONRECORD_H
#define ALCHEMYPOTIONRECORD_H

#include <string>
#include <fstream>
#include <vector>
#include "../Enchantment.h"

struct AlchemyPotionRecord
{
  public:
    std::string AlchemyID;
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

    /* writes the record to the given output stream and returns true on success

      parameters:
          output - the output file stream
    */
    bool saveToStream(std::ofstream& output) const;

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    bool loadFromStream(std::ifstream& in_File);
  protected:
    /*tries to read a subrecord for the alchemy data and returns true on success

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordALDT(std::ifstream& in_File, char* Buffer, int32_t& BytesRead);

    /*tries to read a subrecord for the enchantment and returns true on success

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordENAM(std::ifstream& in_File, char* Buffer, int32_t& BytesRead);

    /*tries to read a subrecord for the item name and returns true on success

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordFNAM(std::ifstream& in_File, char* Buffer, int32_t& BytesRead);

    /*tries to read a subrecord for the script ID and returns true on success

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordSCRI(std::ifstream& in_File, char* Buffer, int32_t& BytesRead);

    /*tries to read a subrecord for the inventory icon and returns true on
      success

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordTEXT(std::ifstream& in_File, char* Buffer, int32_t& BytesRead);
}; //struct

#endif // ALCHEMYPOTIONRECORD_H
