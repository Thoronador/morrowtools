#!/bin/sh

# Script to test executable when parameters are used in the wrong way.
#
#  Copyright (C) 2023, 2025  Dirk Stolle
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

# 2nd parameter = path to test.bsa
if [ -z "$2" ]
then
  echo "Second parameter must be the path to the BSA file!"
  exit 1
fi

BSA_FILE="$2"

# invalid operation given
"$EXECUTABLE" not-an-operation file.bsa
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when an invalid operation was set."
  exit 1
fi

# no arguments given
"$EXECUTABLE"
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when arguments were missing."
  exit 1
fi

# BSA file does not exist / fail to load.
"$EXECUTABLE" info this-file-is-missing.bsa
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when BSA file was missing."
  exit 1
fi

# command file-metadata: --show-total is given twice
"$EXECUTABLE" file-metadata --show-total --show-total file.bsa
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --show-total was given twice."
  exit 1
fi

# command file-metadata: unknown parameter
"$EXECUTABLE" file-metadata "$BSA_FILE" --not-a-valid-parameter
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when an invalid parameter was given."
  exit 1
fi

exit 0
