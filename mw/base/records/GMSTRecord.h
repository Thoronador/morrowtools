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

#ifndef GMSTRECORD_H
#define GMSTRECORD_H

#include <string>
#include "BasicRecord.h"

namespace MWTP
{

enum GMSTType {gtInteger, gtFloat, gtString};

struct GMSTRecord: public BasicRecord
{
  std::string SettingName;
  GMSTType Type;
  int32_t iVal;
  float fVal;
  std::string sVal;

  /* returns true, if the other record contains the same relevant data */
  bool equals(const GMSTRecord& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        out_File - the output file stream
  */
  bool saveToStream(std::ofstream& out_File) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};

} //namespace

#endif // GMSTRECORD_H
