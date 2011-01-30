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

struct SpellRecord
{
  std::string Name;
  int32_t Type;
  int32_t Cost;
  int32_t Flags;
  std::vector<EnchantmentData> Enchs;
  bool saveToStream(std::ofstream& output, const std::string& SpellID) const;
};//struct

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

    /* returns a reference to the spell record of the spell with the gien ID

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

    SpellListIterator getBegin() const;
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
