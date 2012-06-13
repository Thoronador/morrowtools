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

#ifndef MW_SOUNDGENERATORS_H
#define MW_SOUNDGENERATORS_H

#include <string>
#include <map>
#include "records/SoundGeneratorRecord.h"

namespace MWTP
{

//iterator type for sound generator list
typedef std::map<std::string, SoundGeneratorRecord>::const_iterator SoundGeneratorListIterator;

class SoundGenerators
{
  public:
    /* destructor */
    ~SoundGenerators();

    /* singleton access method */
    static SoundGenerators& getSingleton();

    /* adds a sound generator to the list */
    void addSoundGenerator(const SoundGeneratorRecord& record);

    /* returns true, if a sound generator with the given ID is present

       parameters:
           ID - the ID of the sound generator object
    */
    bool hasSoundGenerator(const std::string& ID) const;

    /* returns the number of sound generators in the list */
    unsigned int getNumberOfSoundGenerators() const;

    /* returns a reference to the sound generator record of the sound generator
       with the given ID

       parameters:
           ID - the ID of the sound generator

       remarks:
           If no sound generator with the given ID is present, the function will
           throw an exception. Use hasSoundGenerator() to determine, if a sound
           generator with the desired ID is present.
    */
    const SoundGeneratorRecord& getSoundGenerator(const std::string& ID) const;

    /* tries to read a sound generator record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordSNDG(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    SoundGeneratorListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    SoundGeneratorListIterator getEnd() const;

    /* tries to save all available sound generators to the given stream and
       returns true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    sound generators
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all sound generators from the list */
    void clearAll();
  private:
    /* constructor */
    SoundGenerators();

    /* empty copy constructor */
    SoundGenerators(const SoundGenerators& op) {}

    /* internal data */
    std::map<std::string, SoundGeneratorRecord> m_SoundGenerators;
};//class

} //namespace

#endif // MW_SOUNDGENERATORS_H
