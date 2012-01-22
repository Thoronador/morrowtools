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

#ifndef SR_KEYWORDS_H
#define SR_KEYWORDS_H

#include <string>
#include <map>
#include "records/KeywordRecord.h"

namespace SRTP
{

//iterator type for keyword list
typedef std::map<std::string, KeywordRecord>::const_iterator KeywordListIterator;

class Keywords
{
  public:
    /* destructor */
    ~Keywords();

    /* singleton access method */
    static Keywords& getSingleton();

    /* adds a keyword to the list */
    void addKeyword(const KeywordRecord& record);

    /* returns true, if a keyword with the given ID is present

       parameters:
           ID - the ID of the keyword object
    */
    bool hasKeyword(const std::string& ID) const;

    /* returns the number of keywords in the list */
    unsigned int getNumberOfKeywords() const;

    /* returns a reference to the keyword record of the keyword with the given ID

       parameters:
           ID - the ID of the keyword

       remarks:
           If no keyword with the given ID is present, the function will throw
           an exception. Use hasKeyword() to determine, if a keyword with the
           desired ID is present.
    */
    const KeywordRecord& getKeyword(const std::string& ID) const;

    /* tries to read a keyword record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordKYWD(std::ifstream& in_File);

    /* "alias" for readRecordKYWD(), i.e. calls the above function */
    int readNextRecord(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    KeywordListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    KeywordListIterator getEnd() const;

    #ifndef SR_UNSAVEABLE_RECORDS
    /* tries to save all available keywords to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    keywords
    */
    bool saveAllToStream(std::ofstream& output) const;
    #endif

    /* removes all keywords from the list */
    void clearAll();
  private:
    /* constructor */
    Keywords();

    /* empty copy constructor */
    Keywords(const Keywords& op) {}

    /* internal data */
    std::map<std::string, KeywordRecord> m_Keywords;
};//class

} //namespace

#endif // SR_KEYWORDS_H
