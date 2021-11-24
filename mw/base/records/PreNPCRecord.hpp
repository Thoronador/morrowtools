/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021  Thoronador

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

#ifndef MW_PRENPCRECORD_HPP
#define MW_PRENPCRECORD_HPP

#include <vector>
#include "BasicRecord.hpp"
#include "ItemRecord.hpp"
#include "AIData.hpp"
#include "AIPackages.hpp"
#include "TravelDestination.hpp"

namespace MWTP
{

/** Holds all (AI) data that is common to both NPCs and creatures. */
struct PreNPCRecord: public BasicRecord
{
  PreNPCRecord();

  std::vector<ItemRecord> Items;
  std::vector<std::string> NPC_Spells;
  NPC_AIData AIData;
  std::vector<NPC_BasicAIPackage*> AIPackages;
  std::vector<TravelDestination> Destinations;

  /** Destructor takes care of freeing pointer data. */
  virtual ~PreNPCRecord();

  /** \brief Checks whether another instance contains the same AI packages.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same AI packages.
   *         Returns false otherwise.
   */
  bool hasEqualAIPackages(const PreNPCRecord& other) const;

  /** \brief Checks whether the trainer flag is set in the AIData.
   *
   * \return Returns true, if the trainer flag is set in the AIData.
   */
  bool isTrainer() const;

  /** \brief Checks whether the enchanting flag is set in the AIData.
   *
   * \return Returns true, if the enchanting flag is set in the AIData.
   */
  bool isEnchanter() const;

  /** \brief Checks whether the spellmaking flag is set in the AIData.
   *
   * \return Returns true, if the spellmaking flag is set in the AIData.
   */
  bool isSpellmaker() const;

  /** \brief Checks whether the repair flag is set in the AIData.
   *
   * \return Returns true, if the repair flag is set in the AIData.
   */
  bool doesRepair() const;

  protected:
    /** Removes all AI packages and deletes the pointed to memory. */
    void removeAIPackages();

    /** Copies the AI packages from the source record to this record.
     *
     * \param source  the record to copy the packages from
     */
    void copyAIPackages(const PreNPCRecord& source);

    #ifndef MW_UNSAVEABLE_RECORDS
    bool writeItemsSpellsAIDataDestinations(std::ostream& output) const;
    #endif
}; // struct

} // namespace

#endif // MW_PRENPCRECORD_HPP
