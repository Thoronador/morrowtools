/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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

#ifndef SR_DEPENDENCYSOLVER_H
#define SR_DEPENDENCYSOLVER_H

#include <set>
#include <string>
#include <vector>
#include "records/Tes4HeaderRecord.h"

namespace SRTP
{

struct DependencyElement
{
  std::string name;
  SRTP::Tes4HeaderRecord header;

  DependencyElement()
  : name(""), header(Tes4HeaderRecord())
  { }

  DependencyElement(const std::string& fn)
  : name(fn), header(Tes4HeaderRecord())
  { }

  DependencyElement(const std::string& fn, const Tes4HeaderRecord& head)
  : name(fn), header(head)
  { }

  /* comparison operator */
  bool operator<(const DependencyElement& other) const;
};

bool getLoadOrder(const std::set<DependencyElement>& files, std::vector<std::string>& result);

} //namespace

#endif // SR_DEPENDENCYSOLVER_H
