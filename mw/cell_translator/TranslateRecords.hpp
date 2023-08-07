/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011  Dirk Stolle

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

#ifndef TRANSLATERECORDS_H
#define TRANSLATERECORDS_H

#include "TranslatorXML.hpp"
#include "../../lib/mw/records/CellRecord.hpp"
#include "../../lib/mw/records/DialogueInfoRecord.hpp"
#include "../../lib/mw/records/PreNPCRecord.hpp"
#include "../../lib/mw/records/ScriptRecord.hpp"

namespace MWTP
{

/* tries to translate the cell names in the given NPC/creature record and
   returns true on success, false on failure

   parameters:
       c_rec          - pointer to the record that has to be translated
       cells          - the list of cell names and their translations
       changedRecords - variable that counts the number of changed records. It
                        will be increased by one, if any changes were made to
                        the PreNPCRecord.
*/
bool translatePreNPCRecord(PreNPCRecord* c_rec, const CellListType& cells, unsigned int& changedRecords);

/* tries to translate the cell names in the given cell record and returns true
   on success, false on failure

   parameters:
       c_rec          - pointer to the record that has to be translated
       cells          - the list of cell names and their translations
       changedRecords - variable that counts the number of changed records. It
                        will be increased by one, if any changes were made to
                        the CellRecord.
*/
bool translateCellRecord(CellRecord* c_rec, const CellListType& cells, unsigned int& changedRecords);

/* tries to translate the cell names in the given dialogue info record and
   returns true on success, false on failure

   parameters:
       di_rec         - pointer to the dialogue info record that has to be translated
       cells          - the list of cell names and their translations
       changedRecords - variable that counts the number of changed records. It
                        will be increased by one, if any changes were made to
                        the DialogueInfoRecord.
*/
bool translateInfoRecord(DialogueInfoRecord* di_rec, const CellListType& cells, unsigned int& changedRecords);

/* tries to translate the cell names in the given script record and returns true
   on success, false on failure

   parameters:
       script_rec     - pointer to the ScriptRecord that has to be translated
       cells          - the list of cell names and their translations
       changedRecords - variable that counts the number of changed records. It
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
