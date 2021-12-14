/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#include <iostream>

int main(void)
{
  #if defined(_WIN64)
  // Targeting 64-bit Windows, add compiling statement.
  return 0;
  #else
  // Targeting something else, make compilation fail here.
  // This failure is intentional, because we use CMake's try_compile() command
  // (see <https://cmake.org/cmake/help/latest/command/try_compile.html>) to
  // check whether the build is targeting a 64-bit Windows OS and it needs to
  // fail when that is not the case.
  return fail->hard;
  #endif
}
