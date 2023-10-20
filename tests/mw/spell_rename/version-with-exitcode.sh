#!/bin/sh

# Script to test executable's exit code to indicate its own version.
#
#  Copyright (C) 2023  Dirk Stolle
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

# 1st parameter = executable path
if [ -z "$1" ]
then
  echo "First parameter must be executable file!"
  exit 1
fi

EXECUTABLE="$1"

# non-zero exitcode does not indicate error but the currently used version
"$EXECUTABLE" --version-with-exitcode
CODE=$?

if [ $CODE -eq 0 ]
then
  echo "Executable did exit with code 0 when version was requested."
  exit 1
fi

if [ $CODE -eq 1 ]
then
  echo "Executable did exit with code 1 when version was requested."
  exit 1
fi

# Other codes are fine.
exit 0
