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

#ifndef INGREDIENTS_H
#define INGREDIENTS_H

#include <string>
#include <map>
#include <fstream>

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

struct IngredRec
{
  /* constructor */
  IngredRec();

  //data
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
         IngredID - ID of the ingredient
  */
  bool saveToStream(std::ofstream& output, const std::string& IngredID) const;
};//struct

typedef std::map<const std::string, IngredRec>::const_iterator IngredListIterator;

class Ingredients
{
  public:
    /* destructor */
    ~Ingredients();

    /* singleton access method */
    static Ingredients& getSingleton();

    /* adds an ingredient to the list */
    void addIngredient(const std::string& IngredID, const IngredRec& data);

    /* returns the number of ingredients that are currently in the list */
    unsigned int getNumberOfIngredients() const;

    /* returns true, if an ingredient with the given ID is in the list

       parameters:
           ID - ingredient's ID
    */
    bool hasIngredient(const std::string& ID) const;

    /* returns the ingredient data record of the ingredient with the given ID.
       However, the ingredient has to be in the list. If it isn't, the function
       will throw an exception. Use hasIngredient() to check if an ingredient is
       in the list.

       parameters:
           ID - the ID of the ingredient
    */
    const IngredRec& getIngredientData(const std::string& ID) const;

    /* lists the IDs and names of all ingredients in the list */
    void listAll() const;

    /* returns a constant iterator to the start of the internal list */
    IngredListIterator getBegin() const;

    /* returns a constant iterator to the end of the internal list */
    IngredListIterator getEnd() const;

    /* tries to read an ingredient record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
           FileSize - the total size of the file associated with the stream
    */
    int readRecordINGR(std::ifstream& in_File, const int32_t FileSize);

    /* tries to save all available ingredientss to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the records
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all ingredients from the list */
    void clearAll();
  private:
    /* constructor - private due to singleton pattern */
    Ingredients();

    /* empty copy constructor (singleton pattern) */
    Ingredients(const Ingredients& op) {}

    /* internal data */
    std::map<const std::string, IngredRec> m_Ingredients;
};//class

#endif // INGREDIENTS_H
