#!/bin/sh

# Script to test executable when parameters are used in the wrong way.
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

# path to data files is given twice
"$EXECUTABLE" -d /opt/foo/data -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when data directory was given twice."
  exit 1
fi

# no path after parameter for data files directory
"$EXECUTABLE" -d
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when -d was given without directory path."
  exit 1
fi

# path to data files is too short
"$EXECUTABLE" -d /
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when data directory path was too short."
  exit 1
fi

# no file name after parameter for output file
"$EXECUTABLE" -o
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when -o was given without file name."
  exit 1
fi

# no float value after parameter for scaling factor
"$EXECUTABLE" -s
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when -s was given without scaling value."
  exit 1
fi

# invalid value after parameter for scaling factor
"$EXECUTABLE" -s foo
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when -s was given with non-float value."
  exit 1
fi

# scaling factor too large / too small + invalid parameter
"$EXECUTABLE" -s -1.05 -s 123456 --foo-bar-fail
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when invalid parameters were given."
  exit 1
fi

# no scaling factor / no arguments at all
"$EXECUTABLE"
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when no scaling factor was given."
  exit 1
fi

# unknown / invalid parameter
"$EXECUTABLE" --not-a-known-option
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when an unknown option was given."
  exit 1
fi

exit 0
