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

#ifndef DIALOGUEINFOS_H
#define DIALOGUEINFOS_H

#include <map>
#include "records/DialogueInfoRecord.h"

namespace MWTP
{

//iterator type for dialogue info list
typedef std::map<std::string, DialogueInfoRecord>::const_iterator DialogueInfoListIterator;

class DialogueInfos
{
  public:
    /* destructor */
    ~DialogueInfos();

    /* singleton access method */
    static DialogueInfos& getSingleton();

    /* adds a dialogue info to the list */
    void addDialogueInfo(const DialogueInfoRecord& record);

    /* returns true, if a dialogue info with the given ID is present

       parameters:
           ID - the ID of the dialogue info object
    */
    bool hasDialogueInfo(const std::string& ID) const;

    /* returns the number of dialogue infos in the list */
    unsigned int getNumberOfDialogueInfos() const;

    /* returns a reference to the dialogue info record of the dialogue info
       with the given ID

       parameters:
           ID - the ID of the dialogue info

       remarks:
           If no dialogue info with the given ID is present, the function will
           throw an exception. Use hasDialogueInfo() to determine, if a
           dialogue info with the desired ID is present.
    */
    const DialogueInfoRecord& getDialogueInfo(const std::string& ID) const;

    /* tries to read a dialogue info record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordINFO(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    DialogueInfoListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    DialogueInfoListIterator getEnd() const;

    /* tries to save all available dialogue infos to the given stream and
       returns true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    dialogue infos
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all dialogue infos from the list */
    void clearAll();
  private:
    /* constructor */
    DialogueInfos();

    /* empty copy constructor */
    DialogueInfos(const DialogueInfos& op) {}

    /* internal data */
    std::map<std::string, DialogueInfoRecord> m_DialogueInfos;
};//class

} //namespace

#endif // DIALOGUEINFOS_H
