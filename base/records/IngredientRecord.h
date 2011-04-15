/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published
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

#ifndef INGREDIENTRECORD_H
#define INGREDIENTRECORD_H

#include <string>
#include <fstream>
#include "BasicRecord.h"

/*Ingredients:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    IRDT = Ingredient Data (56 bytes), required
        float  Weight
        long   Value
        long   EffectID[4]	0 or -1 means no effect
        long   SkillID[4]	only for Skill related effects, 0 or -1 otherwise
        long   AttributeID[4]  only for Attribute related effects, 0 or -1 otherwise
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

struct IngredRec: public BasicRecord
{
  public:
    /* constructor */
    IngredRec();

    //data
    std::string IngredID;
    std::string ModelName;
    std::string IngredName;
    //IngredData
    float Weight;
    int32_t Value;
    int32_t EffectID[4];
    int32_t SkillID[4];
    int32_t AttributeID[4];
    // end of Ingred data
    std::string InventoryIcon;
    std::string ScriptName;

    /* shows the content of the record by writing it to the standard output */
    void show();

    /* returns true, if the other record contains the same data */
    bool equals(const IngredRec& other) const;

    /* tries to save the ingredient record to an output file stream and returns
       true on success, false on failure.

       parameters:
           output   - the output file stream that is used to save the record
    */
    bool saveToStream(std::ofstream& output) const;

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    bool loadFromStream(std::ifstream& in_File);
  private:
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
};//struct

#endif // INGREDIENTRECORD_H
