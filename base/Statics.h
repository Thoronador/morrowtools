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

#ifndef STATICS_H
#define STATICS_H

#include <string>
#include <map>
#include <fstream>

struct StaticRecord
{
  std::string Mesh;

  /* returns true, if the other record contains the same data */
  bool equals(const StaticRecord& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output file stream
  */
  bool saveToStream(std::ofstream& output, const std::string& StaticID) const;
};//struct

//iterator type for static list
typedef std::map<std::string, StaticRecord>::const_iterator StaticListIterator;

class Statics
{
  public:
    /* destructor */
    ~Statics();

    /* singleton access method */
    static Statics& getSingleton();

    /* adds a static to the list */
    void addStatic(const std::string& ID, const StaticRecord& record);

    /* returns true, if a static with the given ID is present

       parameters:
           ID - the ID of the static object
    */
    bool hasStatic(const std::string& ID) const;

    /* returns the number of statics in the list */
    unsigned int getNumberOfStatics() const;

    /* returns a reference to the static record of the static with the given ID

       parameters:
           ID - the ID of the static

       remarks:
           If no static with the given ID is present, the function will throw an
           exception. Use hasStatic() to determine, if a static with the desired
           ID is present.
    */
    const StaticRecord& getStatic(const std::string& ID) const;

    /* tries to read a statics record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File - the input file stream that is used to read the record
    */
    int readRecordSTAT(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    StaticListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    StaticListIterator getEnd() const;

    /* tries to save all available statics to the given stream and returns true
       on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the statics
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all statics from the list */
    void clearAll();
  private:
    /* constructor */
    Statics();

    /* empty copy constructor */
    Statics(const Statics& op) {}

    /* internal data */
    std::map<std::string, StaticRecord> m_Statics;
};//class

#endif // STATICS_H
