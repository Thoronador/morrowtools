/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#ifndef ESMREADER_H
#define ESMREADER_H

#include <string>
#include <fstream>

bool ReadESM(const std::string& FileName, const bool verbose);

//"dispatcher"
bool ProcessNextRecord(std::ifstream& in_File, const int32_t FileSize);

class ESMReader
{
  public:
    /* constructor */
    ESMReader();

    /* destructor */
    ~ESMReader();

    /* skips a record and returns 0 on success, and -1 on error */
    static int skipRecord(std::ifstream& in_File);
};//class

#endif // ESMREADER_H
