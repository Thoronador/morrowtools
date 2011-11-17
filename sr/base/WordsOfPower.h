/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_WORDSOFPOWER_H
#define SR_WORDSOFPOWER_H

#include <map>
#include <string>
#include "records/WordOfPowerRecord.h"

namespace SRTP
{

//iterator type for word of power list
typedef std::map<std::string, WordOfPowerRecord>::const_iterator WordOfPowerListIterator;

class WordsOfPower
{
  public:
    /* destructor */
    ~WordsOfPower();

    /* singleton access method */
    static WordsOfPower& getSingleton();

    /* adds a word of power to the list */
    void addWordOfPower(const WordOfPowerRecord& record);

    /* returns true, if a word of power with the given ID is present

       parameters:
           ID - the ID of the word of power
    */
    bool hasWordOfPower(const std::string& ID) const;

    /* returns the number of words of power in the list */
    unsigned int getNumberOfWordsOfPower() const;

    /* returns a reference to the word of power record with the given ID

       parameters:
           ID - the ID of the word of power

       remarks:
           If no word of power with the given ID is present, the function will
           throw an exception. Use hasWordOfPower() to determine, if a word of
           power with the desired ID is present.
    */
    const WordOfPowerRecord& getWordOfPower(const std::string& ID) const;

    /* tries to read a word of power record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readNextRecord(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    WordOfPowerListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    WordOfPowerListIterator getEnd() const;

    /* tries to save all available words to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    words of power
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all words from the list */
    void clearAll();
  private:
    /* constructor */
    WordsOfPower();

    /* empty copy constructor */
    WordsOfPower(const WordsOfPower& op) {}

    /* internal data */
    std::map<std::string, WordOfPowerRecord> m_WordsOfPower;
};//class

} //namespace

#endif // SR_WORDSOFPOWER_H
