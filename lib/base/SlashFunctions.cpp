/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2021, 2024  Dirk Stolle

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

#include "SlashFunctions.hpp"

namespace MWTP
{

std::string flipBackslashes(std::string s)
{
  for (auto pos = s.find('\\'); pos != std::string::npos; pos = s.find('\\', pos))
  {
     s.replace(pos, 1, 1, '/');
  }
  return s;
}

std::string flipForwardSlashes(std::string s)
{
  for (auto pos = s.find('/'); pos != std::string::npos; pos = s.find('/', pos))
  {
     s.replace(pos, 1, 1, '\\');
  }
  return s;
}

} // namespace
