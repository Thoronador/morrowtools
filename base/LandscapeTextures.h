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

#ifndef LANDSCAPETEXTURES_H
#define LANDSCAPETEXTURES_H

#include <string>
#include <map>
#include "records/LandscapeTextureRecord.h"

//iterator type for landscape texture list
typedef std::map<std::string, LandscapeTextureRecord>::const_iterator LandscapeTextureListIterator;

class LandscapeTextures
{
  public:
    /* destructor */
    ~LandscapeTextures();

    /* singleton access method */
    static LandscapeTextures& getSingleton();

    /* adds a landscape texture to the list */
    void addLandscapeTexture(const std::string& ID, const LandscapeTextureRecord& record);

    /* returns true, if a landscape texture with the given ID is present

       parameters:
           ID - the ID of the landscape texture object
    */
    bool hasLandscapeTexture(const std::string& ID) const;

    /* returns the number of landscape textures in the list */
    unsigned int getNumberOfLandscapeTextures() const;

    /* returns a reference to the landscape texture record of the landscape
       texture with the given ID

       parameters:
           ID - the ID of the landscape texture

       remarks:
           If no landscape texture with the given ID is present, the function
           will throw an exception. Use hasLandscapeTexture() to determine, if
           a landscape texture with the desired ID is present.
    */
    const LandscapeTextureRecord& getLandscapeTexture(const std::string& ID) const;

    /* tries to read a landscape texture record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File  - the input file stream that is used to read the record
    */
    int readRecordLTEX(std::ifstream& in_File);

    /* returns constant iterator to the beginning of the internal list */
    LandscapeTextureListIterator getBegin() const;

    /* returns constant iterator to the end of the internal list */
    LandscapeTextureListIterator getEnd() const;

    /* tries to save all available landscape textures to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the
                    landscape textures
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all landscape textures from the list */
    void clearAll();
  private:
    /* constructor */
    LandscapeTextures();

    /* empty copy constructor */
    LandscapeTextures(const LandscapeTextures& op) {}

    /* internal data */
    std::map<std::string, LandscapeTextureRecord> m_LandscapeTextures;
};//class

#endif // LANDSCAPETEXTURES_H
