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

# --male is given twice
"$EXECUTABLE" --male --male
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --male was given twice."
  exit 1
fi

# --female is given twice
"$EXECUTABLE" --female --female
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --female was given twice."
  exit 1
fi

# --both is given twice
"$EXECUTABLE" --both --both
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --both was given twice."
  exit 1
fi

# count is given twice
"$EXECUTABLE" --count 5 --count 8
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when count was given twice."
  exit 1
fi

# no number after parameter for count
"$EXECUTABLE" --count
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --count was given without a number."
  exit 1
fi

# count is less than one
"$EXECUTABLE" --count -3
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when count was less than one."
  exit 1
fi

# count is not a valid number
"$EXECUTABLE" --count foo34
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when count was not a number."
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
