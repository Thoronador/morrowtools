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

#ifndef TRANSLATERECORDS_H
#define TRANSLATERECORDS_H

#include "TranslatorXML.h"
#include "../base/records/CellRecord.h"
#include "../base/records/DialogueInfoRecord.h"
#include "../base/records/PreNPCRecord.h"
#include "../base/records/ScriptRecord.h"

namespace MWTP
{

/* tries to translate the cell names in the given NPC/creature record and
   returns true on success, false on failure

   parameters:
       c_rec          - pointer to the record that has to be translated
       cells          - the list of cell names and their translations
       changedRecords - variable that counts the number of changed record. It
                        will be increased by one, if any changes were made to
                        the PreNPCRecord.
*/
bool translatePreNPCRecord(PreNPCRecord* c_rec, const CellListType& cells, unsigned int& changedRecords);

/* tries to translate the cell names in the given cell record and returns true
   on success, false on failure

   parameters:
       c_rec          - pointer to the record that has to be translated
       cells          - the list of cell names and their translations
       changedRecords - variable that counts the number of changed record. It
                        will be increased by one, if any changes were made to
                        the CellRecord.
*/
bool translateCellRecord(CellRecord* c_rec, const CellListType& cells, unsigned int& changedRecords);

/* tries to translate the cell names in the given dialogue info record and
   returns true on success, false on failure

   parameters:
       di_rec         - pointer to the dialogue info record that has to be translated
       cells          - the list of cell names and their translations
       changedRecords - variable that counts the number of changed record. It
                        will be increased by one, if any changes were made to
                        the DialogueInfoRecord.
*/
bool translateInfoRecord(DialogueInfoRecord* di_rec, const CellListType& cells, unsigned int& changedRecords);

/* tries to translate the cell names in the given script record and returns true
   on success, false on failure

   parameters:
       script_rec     - pointer to the ScriptRecord that has to be translated
       cells          - the list of cell names and their translations
       changedRecords - variable that counts the number of changed record. It
                        will be increased by one, if any changes were made to
                        the ScriptRecord.
*/
//bool translateScriptRecord(ScriptRecord* script_rec, const CellListType& cells, unsigned int& changedRecords);

/* tries to translate the script text given in scriptText, and returns true, if
   something had to be (and was) changed, or false if nothing was changed

   parameters:
       scriptText - reference to the script text that has to be translated
       cells      - the list of cell names and their translations
*/
bool replaceCellsInScriptText(std::string& scriptText, const CellListType& cells);

/* checks, if the script compiler can compile the given script and produce the
   exact script data that is in the original script record. If so, the function
   returns true. Otherwise it returns false.

   remarks:
       That function relies on the fact that the record contains valid and
       unchanged script data, to compare the result to the given data.
*/
bool canCompileScriptProperly(const ScriptRecord& original);

} //namespace

#endif // TRANSLATERECORDS_H
