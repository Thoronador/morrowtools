/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2025  Dirk Stolle

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

#ifndef MW_TRANSLATERECORDS_HPP
#define MW_TRANSLATERECORDS_HPP

#include "TranslatorXML.hpp"
#include "../../../lib/mw/records/CellRecord.hpp"
#include "../../../lib/mw/records/DialogueInfoRecord.hpp"
#include "../../../lib/mw/records/PreNPCRecord.hpp"
#include "../../../lib/mw/records/ScriptRecord.hpp"

namespace MWTP
{

/** \brief Tries to translate the cell names in the given NPC/creature record.
 *
 * \param c_rec           pointer to the record that has to be translated
 * \param cells           the list of cell names and their translations
 * \param changedRecords  variable that counts the number of changed records. It
 *                         will be increased by one, if any changes were made to
 *                         the PreNPCRecord.
 * \return Returns true on success, false on failure.
 */
bool translatePreNPCRecord(PreNPCRecord* c_rec, const CellListType& cells, unsigned int& changedRecords);

/** \brief Tries to translate the cell names in the given cell record.
 *
 * \param c_rec           pointer to the record that has to be translated
 * \param cells           the list of cell names and their translations
 * \param changedRecords  variable that counts the number of changed records. It
 *                        will be increased by one, if any changes were made to
 *                        the CellRecord.
 * \return Returns true on success, false on failure.
 */
bool translateCellRecord(CellRecord* c_rec, const CellListType& cells, unsigned int& changedRecords);

/** \brief Tries to translate the cell names in the given dialogue info record.
 *
 * \param di_rec          pointer to the dialogue info record that has to be translated
 * \param cells           the list of cell names and their translations
 * \param changedRecords  variable that counts the number of changed records. It
 *                        will be increased by one, if any changes were made to
 *                        the DialogueInfoRecord.
 * \return Returns true on success, false on failure.
 */
bool translateInfoRecord(DialogueInfoRecord* di_rec, const CellListType& cells, unsigned int& changedRecords);

/** \brief Tries to translate the cell names in the given script record.
 *
 * \param script_rec      pointer to the ScriptRecord that has to be translated
 * \param cells           the list of cell names and their translations
 * \param changedRecords  variable that counts the number of changed records. It
 *                        will be increased by one, if any changes were made to
 *                        the ScriptRecord.
 * \return Returns true on success, false on failure.
 */
//bool translateScriptRecord(ScriptRecord* script_rec, const CellListType& cells, unsigned int& changedRecords);

/** \brief Tries to translate the script text given in script text.
 *
 * \param scriptText  reference to the script text that has to be translated
 * \param cells       the list of cell names and their translations
 * \return Returns true, if something had to be (and was) changed.
 *         Returns false, if nothing was changed
 */
bool replaceCellsInScriptText(std::string& scriptText, const CellListType& cells);

/** \brief Checks, if the script compiler can compile the given script and
 * produce the exact script data that is in the original script record.
 * \return If the script can be compiled exactly, the function returns true.
 *         Otherwise it returns false.
 * \remarks The function relies on the fact that the record contains valid and
 *          unchanged script data, to compare the result to the given data.
*/
bool canCompileScriptProperly(const ScriptRecord& original);

} // namespace

#endif // MW_TRANSLATERECORDS_HPP
