/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#ifndef SPELLS_H
#define SPELLS_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "Enchantment.h"

const int32_t stSpell = 0;
const int32_t stAbility = 1;
const int32_t stBlight = 2;
const int32_t stDisease = 3;
const int32_t stCurse = 4;
const int32_t stPower = 5;

const int32_t sfAutoCalcCosts = 1;
const int32_t sfPCStartSpell = 2;
const int32_t sfAlwaysSucceeds = 4;

struct SpellRecord
{
  std::string Name;
  int32_t Type;
  int32_t Cost;
  int32_t Flags;
  std::vector<EnchantmentData> Enchs;

  /* tries to save the spell record to an output file stream and returns true
     on success, false on failure.

     parameters:
         output  - the output file stream that is used to save the spell data
         SpellID - ID of the spell
  */
  bool saveToStream(std::ofstream& output, const std::string& SpellID) const;

  /* returns true, if the other spell record contains the same data */
  bool equals(const SpellRecord& other) const;

  /* returns true, if the Auto Calculate Costs flag is set */
  bool autoCalculateCosts() const;

  /* returns true, if the spell is a start spell */
  bool isPCStartSpell() const;

  /* returns true, if the "always succeed" flag is set */
  bool alwaysSucceeds() const;
};//struct

//iterator type for Spells class
typedef std::map<std::string, SpellRecord>::const_iterator SpellListIterator;

class Spells
{
  public:
    /* destructor */
    ~Spells();

    /* singleton access method */
    static Spells& getSingleton();

    /* adds a spell to the list */
    void addSpell(const std::string& ID, const SpellRecord& record);

    /* returns true, if a spell with the given ID exists

       parameters:
           ID - the ID of the spell
    */
    bool hasSpell(const std::string& ID) const;

    /* returns the number of spells in the list */
    unsigned int getNumberOfSpells() const;

    /* returns a reference to the spell record of the spell with the given ID

       parameters:
           ID - the ID of the spell

       remarks:
           If no spell with the given ID is present, the function will throw an
           exception. Use hasSpell() to determine, if a spell with the desired
           ID is present.
    */
    const SpellRecord& getSpell(const std::string& ID) const;

    /* tries to read one spell record (without first 32 bits of header) from the
       given input file stream and returns true on success, false on error

       parameters:
           in_File - the input file stream that is used for the operation
    */
    bool readSPEL(std::ifstream& in_File);

    /* tries to read a spell record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File - the input file stream that is used to read the record
    */
    int readRecordSPEL(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    SpellListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    SpellListIterator getEnd() const;

    /* tries to save all available spells to the given stream and returns true
       on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the spells
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes the spell with the given ID from the list and returns true, if
       such a spell was present

       parameters:
           ID - the ID of the spell that shall be removed
    */
    bool removeSpell(const std::string& ID);

    /* removes all spells from the list */
    void clearAll();

    /* constant that holds the maximum length of spell name, that Morrowind can
       still handle properly (excluding terminating NUL character)
    */
    static const size_t cMaximumSpellNameLength;
  private:
    /* constructor */
    Spells();

    /* compy constructor - empty due to singleton pattern */
    Spells(const Spells& op) {}

    /* internal spell data list */
    std::map<std::string, SpellRecord> m_Spells;
}; //class

#endif // SPELLS_H
