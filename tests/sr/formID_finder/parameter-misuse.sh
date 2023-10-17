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

# search keyword is given twice
"$EXECUTABLE" --keyword bow --keyword bow -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when keyword was given twice."
  exit 1
fi

# no keyword after parameter for search keyword
"$EXECUTABLE" --keyword
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when --keyword was given without actual keyword."
  exit 1
fi

# case-sensitivity is set twice
"$EXECUTABLE" --case-sensitive --case-sensitive -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when case-sensitivity was given twice."
  exit 1
fi

# quest information option is set twice
"$EXECUTABLE" --all-quest-info --all-quest-info -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when quest info option was given twice."
  exit 1
fi

# faction rank option is set twice
"$EXECUTABLE" --faction-ranks --faction-ranks -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when faction ranks option was given twice."
  exit 1
fi

# Special Edition option is set twice
"$EXECUTABLE" --special-edition --special-edition -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when Special Edition option was given twice."
  exit 1
fi

# old edition option is set twice
"$EXECUTABLE" --original-edition --original-edition -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when Original Edition option was given twice."
  exit 1
fi

# English language is set twice
"$EXECUTABLE" --en --en -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when English language was given twice."
  exit 1
fi

# French language is set twice
"$EXECUTABLE" --fr --french -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when French language was given twice."
  exit 1
fi

# German language is set twice
"$EXECUTABLE" --de --german -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when German language was given twice."
  exit 1
fi

# Italian language is set twice
"$EXECUTABLE" --it --italian -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when Italian language was given twice."
  exit 1
fi

# Polish language is set twice
"$EXECUTABLE" --pl --polish -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when Polish language was given twice."
  exit 1
fi

# Russian language is set twice
"$EXECUTABLE" --ru --russian -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when Russian language was given twice."
  exit 1
fi

# Spanish language is set twice
"$EXECUTABLE" --es --spanish -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when Spanish language was given twice."
  exit 1
fi

# reference ids are requested twice
"$EXECUTABLE" --ref-id --ref-id -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when reference ids were requested twice."
  exit 1
fi

# file names are requested twice
"$EXECUTABLE" --show-files --show-files -d /opt/foo/data
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when file names were requested twice."
  exit 1
fi

# no arguments given
"$EXECUTABLE"
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when arguments were missing."
  exit 1
fi

# no search keyword given
"$EXECUTABLE" --ref-id
if [ $? -ne 1 ]
then
  echo "Executable did not exit with code 1 when search keyword was missing."
  exit 1
fi

exit 0
