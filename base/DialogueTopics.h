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

#ifndef DIALOGUETOPICS_H
#define DIALOGUETOPICS_H

#include <map>
#include "records/DialogueTopicRecord.h"

//iterator type for dialogue topic list
typedef std::map<std::string, DialogueTopicRecord>::const_iterator DialogueTopicListIterator;

class DialogueTopics
{
  public:
    /* destructor */
    ~DialogueTopics();

    /* singleton access method */
    static DialogueTopics& getSingleton();

    /* adds a dialogue topic to the list */
    void addDialogueTopic(const DialogueTopicRecord& record);

    /* returns true, if a dialogue topic with the given ID is present

       parameters:
           ID - the ID of the dialogue topic object
    */
    bool hasDialogueTopic(const std::string& ID) const;

    /* returns the number of dialogue topics in the list */
    unsigned int getNumberOfDialogueTopics() const;

    /* returns a reference to the dialogue topic record of the dialogue topic
       with the given ID

       parameters:
           ID - the ID of the dialogue topic

       remarks:
           If no dialogue topic with the given ID is present, the function will
           throw an exception. Use hasDialogueTopic() to determine, if a
           dialogue topic with the desired ID is present.
    */
    const DialogueTopicRecord& getDialogueTopic(const std::string& ID) const;

    /* tries to read a dialogue topic record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordDIAL(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    DialogueTopicListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    DialogueTopicListIterator getEnd() const;

    /* tries to save all available dialogue topics to the given stream and
       returns true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    dialogue topics
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all dialogue topics from the list */
    void clearAll();
  private:
    /* constructor */
    DialogueTopics();

    /* empty copy constructor */
    DialogueTopics(const DialogueTopics& op) {}

    /* internal data */
    std::map<std::string, DialogueTopicRecord> m_DialogueTopics;
};//class

#endif // DIALOGUETOPICS_H
