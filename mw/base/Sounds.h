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

#ifndef SOUNDS_H
#define SOUNDS_H

#include <map>
#include "records/SoundRecord.h"

namespace MWTP
{

//iterator type for sound list
typedef std::map<std::string, SoundRecord>::const_iterator SoundListIterator;

class Sounds
{
  public:
    /* destructor */
    ~Sounds();

    /* singleton access method */
    static Sounds& getSingleton();

    /* adds a sound to the list */
    void addSound(const SoundRecord& record);

    /* returns true, if a sound with the given ID is present

       parameters:
           ID - the ID of the sound object
    */
    bool hasSound(const std::string& ID) const;

    /* returns the number of sounds in the list */
    unsigned int getNumberOfSounds() const;

    /* returns a reference to the sound record of the sound with the
       given ID

       parameters:
           ID - the ID of the sound

       remarks:
           If no sound with the given ID is present, the function will throw
           an exception. Use hasSound() to determine, if a sound with the
           desired ID is present.
    */
    const SoundRecord& getSound(const std::string& ID) const;

    /* tries to read a sound record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordSOUN(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    SoundListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    SoundListIterator getEnd() const;

    /* tries to save all available sounds to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    sounds
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all sounds from the list */
    void clearAll();
  private:
    /* constructor */
    Sounds();

    /* empty copy constructor */
    Sounds(const Sounds& op) {}

    /* internal data */
    std::map<std::string, SoundRecord> m_Sounds;
};//class

}//namespace

#endif // SOUNDS_H
