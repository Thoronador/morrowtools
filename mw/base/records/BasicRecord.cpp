/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "BasicRecord.hpp"

namespace MWTP
{

const int32_t FlagPersists = 1024;
const int32_t FlagBlocked = 8192;

BasicRecord::BasicRecord()
: HeaderOne(0),
  HeaderFlags(0)
{
}

BasicRecord::~BasicRecord()
{
  //empty
  //Purpose is to have a virtual destructor to get rid of compiler warnings.
}

int32_t BasicRecord::getHeaderOne() const
{
  return HeaderOne;
}

int32_t BasicRecord::getHeaderFlags() const
{
  return HeaderFlags;
}

void BasicRecord::setHeaderOne(const int32_t newValue)
{
  HeaderOne = newValue;
}

void BasicRecord::setHeaderFlags(const int32_t newValue)
{
  HeaderFlags = newValue;
}

bool BasicRecord::isBlocked() const
{
  return ((HeaderFlags & FlagBlocked)!=0);
}

bool BasicRecord::isPersistent() const
{
  return ((HeaderFlags & FlagPersists)!=0);
}

} //namespace
