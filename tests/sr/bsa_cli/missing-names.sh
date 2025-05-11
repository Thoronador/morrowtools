#!/bin/sh

# Script to test executable when archive has no file and directory names.
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

# 2nd parameter = path to test.bsa
if [ -z "$2" ]
then
  echo "Second parameter must be the path to the BSA file!"
  exit 1
fi

BSA_FILE="$2"

# command check-hashes
"$EXECUTABLE" check-hashes "$BSA_FILE"
if [ $? -ne 3 ]
then
  echo "Executable (running command check-hashes) did not exit with code 3 when BSA name data was missing."
  exit 1
fi

# command directories
"$EXECUTABLE" directories "$BSA_FILE"
if [ $? -ne 3 ]
then
  echo "Executable (running command directories) did not exit with code 3 when BSA name data was missing."
  exit 1
fi

# command directory-metadata
"$EXECUTABLE" directory-metadata "$BSA_FILE"
if [ $? -ne 3 ]
then
  echo "Executable (running command directory-metadata) did not exit with code 3 when BSA name data was missing."
  exit 1
fi

# command extract-all
"$EXECUTABLE" extract-all "$BSA_FILE" /tmp/extract-here
if [ $? -ne 3 ]
then
  echo "Executable (running command extract-all) did not exit with code 3 when BSA name data was missing."
  exit 1
fi

# command extract-directory
"$EXECUTABLE" extract-directory "$BSA_FILE" 'meshes\foo\bar' /tmp/extract-here
if [ $? -ne 3 ]
then
  echo "Executable (running command extract-directory) did not exit with code 3 when BSA name data was missing."
  exit 1
fi

# command extract-file
"$EXECUTABLE" extract-file "$BSA_FILE" 'meshes\foo\bar.dds' /tmp/extract-here/file.dds
if [ $? -ne 3 ]
then
  echo "Executable (running command extract-file) did not exit with code 3 when BSA name data was missing."
  exit 1
fi

# command file-metadata
"$EXECUTABLE" file-metadata "$BSA_FILE"
if [ $? -ne 3 ]
then
  echo "Executable (running command file-metadata) did not exit with code 3 when BSA name data was missing."
  exit 1
fi

# command list
"$EXECUTABLE" list "$BSA_FILE"
if [ $? -ne 3 ]
then
  echo "Executable (running command list) did not exit with code 3 when BSA name data was missing."
  exit 1
fi

exit 0
