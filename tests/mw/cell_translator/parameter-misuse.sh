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

# plugin to translate is given twice
"$EXECUTABLE" -f file.esp -f plugin.esp
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when plugin file was given twice."
  exit 1
fi

# no file name after parameter for plugin file
"$EXECUTABLE" -f
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when -f was given without file name."
  exit 1
fi

# output file is given twice
"$EXECUTABLE" -o output.esp -o output.esp
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when output file was given twice."
  exit 1
fi

# no file name after parameter for output file
"$EXECUTABLE" -o
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when -o was given without file name."
  exit 1
fi

# --force is given twice
"$EXECUTABLE" --force --force
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --force was given twice."
  exit 1
fi

# XML file is given twice
"$EXECUTABLE" -xml data.xml -xml translation.xml
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when XML file was given twice."
  exit 1
fi

# no file name after parameter for XML file
"$EXECUTABLE" -xml
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when -xml was given without file name."
  exit 1
fi

# --no-scripts is given twice
"$EXECUTABLE" --no-scripts --no-scripts
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --no-scripts was given twice."
  exit 1
fi

# --dare is given twice
"$EXECUTABLE" --dare --dare
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --dare was given twice."
  exit 1
fi

# --deutsch is given twice
"$EXECUTABLE" --deutsch -de
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --deutsch was given twice."
  exit 1
fi

# --en is given twice
"$EXECUTABLE" -en -en
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when -en was given twice."
  exit 1
fi

# no plugin file given
"$EXECUTABLE" --english
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when no plugin file was given."
  exit 1
fi

# plugin file and output file are identical
"$EXECUTABLE" -f foo.esp -o foo.esp
if [ $? -ne 10 ]
then
  echo "Executable did not exit with code 10 when plugin file and output file were identical."
  exit 1
fi

# output file is the Morrowind master file
"$EXECUTABLE" -f foo.esp -o Morrowind.esm
if [ $? -ne 10 ]
then
  echo "Executable did not exit with code 10 when an invalid output file was given."
  exit 1
fi

# output file is the Tribunal master file
"$EXECUTABLE" -f foo.esp -o Tribunal.esm
if [ $? -ne 10 ]
then
  echo "Executable did not exit with code 10 when an invalid output file (TR) was given."
  exit 1
fi

# output file is the Bloodmoon master file
"$EXECUTABLE" -f foo.esp -o Bloodmoon.esm
if [ $? -ne 10 ]
then
  echo "Executable did not exit with code 10 when an invalid output file (BM) was given."
  exit 1
fi

# no arguments at all
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
