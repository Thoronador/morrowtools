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
"$EXECUTABLE" -d /opt/foo/data -d /opt/foo/data -p plugin.esp
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
"$EXECUTABLE" -d / -p plugin.esp
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when data directory path was too short."
  exit 1
fi

# plugin file is given twice
"$EXECUTABLE" --plugin-file plugin.esp --plugin-file plugin.esp
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when plugin was given twice."
  exit 1
fi

# no file name after parameter for plugin file
"$EXECUTABLE" --plugin-file
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --plugin-file was given without file name."
  exit 1
fi

# name of plugin is too short
"$EXECUTABLE" --plugin-file a.b
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when plugin file name was too short."
  exit 1
fi

# no arguments given
"$EXECUTABLE"
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when arguments were missing."
  exit 1
fi

# no plugin file given
"$EXECUTABLE" -d /opt/foo/bar
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when plugin file was missing."
  exit 1
fi

# Special Edition option is set twice
"$EXECUTABLE" --special-edition --special-edition -p plugin.esp
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when Special Edition option was given twice."
  exit 1
fi

# old edition option is set twice
"$EXECUTABLE" --original-edition --original-edition -p plugin.esp
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when Original Edition option was given twice."
  exit 1
fi

exit 0
