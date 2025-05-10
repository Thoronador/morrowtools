/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2021, 2022, 2025  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "BodyPartAssociation.hpp"
#ifndef MW_UNSAVEABLE_RECORDS
#include "../MW_Constants.hpp"
#endif

namespace MWTP
{

BodyPartAssociation::BodyPartAssociation()
: Index(0),
  MaleBodyPart(std::string()),
  FemaleBodyPart(std::string())
{
}

bool BodyPartAssociation::operator==(const BodyPartAssociation& other) const
{
  return (Index == other.Index) && (MaleBodyPart == other.MaleBodyPart)
      && (FemaleBodyPart == other.FemaleBodyPart);
}

#ifndef MW_UNSAVEABLE_RECORDS
uint32_t BodyPartAssociation::getWriteSize() const
{
  uint32_t writeSize = 4 /* INDX */ + 4 /* 4 bytes for length */ + 1 /* data: 1 byte */;
  if (!MaleBodyPart.empty())
  {
    writeSize = writeSize + 4 /* BNAM */ + 4 /* 4 bytes for length */
              + MaleBodyPart.length() /* length of data, no NUL terminator */;
  }
  if (!FemaleBodyPart.empty())
  {
    writeSize = writeSize + 4 /* CNAM */ + 4 /* 4 bytes for length */
              + FemaleBodyPart.length() /* length of data, no NUL terminator */;
  }
  return writeSize;
}

bool BodyPartAssociation::saveToStream(std::ostream& output) const
{
  // write body part index (INDX)
  output.write(reinterpret_cast<const char*>(&cINDX), 4);
  uint32_t subLength = 1;
  output.write(reinterpret_cast<const char*>(&subLength), 4);
  output.write(reinterpret_cast<const char*>(&Index), 1);

  if (!MaleBodyPart.empty())
  {
    // write male body part name (BNAM)
    output.write(reinterpret_cast<const char*>(&cBNAM), 4);
    subLength = MaleBodyPart.length();
    output.write(reinterpret_cast<const char*>(&subLength), 4);
    output.write(MaleBodyPart.c_str(), subLength);
  }
  if (!FemaleBodyPart.empty())
  {
    // write female body part name (CNAM)
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    subLength = FemaleBodyPart.length();
    output.write(reinterpret_cast<const char*>(&subLength), 4);
    output.write(FemaleBodyPart.c_str(), subLength);
  }

  return output.good();
}
#endif

} // namespace
