/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2021  Dirk Stolle

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

#ifndef MW_INGREDIENTRECORD_HPP
#define MW_INGREDIENTRECORD_HPP

#include <array>
#include <string>
#include "BasicRecord.hpp"

namespace MWTP
{

/** Holds information about an ingredient. */
struct IngredientRecord: public BasicRecord
{
  public:
    IngredientRecord();

    std::string recordID;
    std::string ModelPath;
    std::string Name;
    // IngredientData
    float Weight;
    int32_t Value;
    std::array<int32_t, 4> EffectID;
    std::array<int32_t, 4> SkillID;
    std::array<int32_t, 4> AttributeID;
    // end of Ingredient data
    std::string InventoryIcon;
    std::string ScriptID;

    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as this instance.
     *         Returns false otherwise.
     */
    bool equals(const IngredientRecord& other) const;

    #ifndef MW_UNSAVEABLE_RECORDS
    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    bool saveToStream(std::ostream& output) const override;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param input    the input stream
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    bool loadFromStream(std::istream& input) override;
}; // struct

} // namespace

#endif // MW_INGREDIENTRECORD_HPP
