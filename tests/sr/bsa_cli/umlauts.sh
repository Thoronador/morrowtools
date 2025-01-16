#!/bin/sh

# Script to test executable when a parameter contains umlauts.
#
#  Copyright (C) 2025  Dirk Stolle
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

# 2nd parameter = path to Möwe.bsa
if [ -z "$2" ]
then
  echo "Second parameter must be the path to the BSA file!"
  exit 1
fi

BSA_FILE="$2"

# check hashes
"$EXECUTABLE" check-hashes "$BSA_FILE"
if [ $? -ne 0 ]
then
  echo "Executable did not exit with code 0 when check-hashes operation was executed."
  exit 1
fi

# directories
"$EXECUTABLE" directories "$BSA_FILE"
if [ $? -ne 0 ]
then
  echo "Executable did not exit with code 0 when directories operation was executed."
  exit 1
fi

# directory-metadata
"$EXECUTABLE" directory-metadata "$BSA_FILE"
if [ $? -ne 0 ]
then
  echo "Executable did not exit with code 0 when directory-metadata operation was executed."
  exit 1
fi

# file-metadata
"$EXECUTABLE" file-metadata "$BSA_FILE"
if [ $? -ne 0 ]
then
  echo "Executable did not exit with code 0 when file-metadata operation was executed."
  exit 1
fi

# info
"$EXECUTABLE" info "$BSA_FILE"
if [ $? -ne 0 ]
then
  echo "Executable did not exit with code 0 when info operation was executed."
  exit 1
fi

# list
"$EXECUTABLE" list "$BSA_FILE"
if [ $? -ne 0 ]
then
  echo "Executable did not exit with code 0 when list operation was executed."
  exit 1
fi

exit 0
