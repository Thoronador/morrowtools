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

#ifndef BOOKRECORD_H
#define BOOKRECORD_H

#include <string>
#include <fstream>
#include "BasicRecord.h"

struct BookRecord: public BasicRecord
{
  public:
    std::string BookID;
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

    /* alternative constructor */
    BookRecord(const std::string& ID);

    /* returns true, if the other record contains the same data */
    bool equals(const BookRecord& other) const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    bool saveToStream(std::ofstream& output) const;

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    bool loadFromStream(std::ifstream& in_File);

  protected:
    /*tries to read the subrecord for the inventory icon and returns true on
      success, false on failure

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordITEX(std::ifstream& in_File, char* Buffer, uint32_t& BytesRead);

    /*tries to read the subrecord for the script ID and returns true on success

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordSCRI(std::ifstream& in_File, char* Buffer, uint32_t& BytesRead);

    /*tries to read a subrecord for the enchantment ID and returns true on
      success

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to a buffer of at least 256 bytes length
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordENAM(std::ifstream& in_File, char* Buffer, uint32_t& BytesRead);

    /*tries to read the subrecord for the book's text

      parameters:
          in_File   - the input file stream
          Buffer    - pointer to the pointer a buffer
          BytesRead - reference to a variable that counts the number of bytes
                      that were read from the stream
    */
    bool readSubRecordTEXT(std::ifstream& in_File, char** Buffer, uint32_t& BytesRead);
};//struct

//comparison operator for BookRecords
bool operator<(const BookRecord& left, const BookRecord& right);

#endif // BOOKRECORD_H
