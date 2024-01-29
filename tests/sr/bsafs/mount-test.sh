#!/bin/sh

# Script to test mounting with bsafs.
#
#  Copyright (C) 2024  Dirk Stolle
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

# 2nd parameter = path of BSA file
if [ -z "$2" ]
then
  echo "Second parameter must be BSA file!"
  exit 1
fi

if [ ! -f "$2" ]
then
  echo "Second parameter must be an existing BSA file!"
  exit 1
fi

EXECUTABLE="$1"
BSA_FILE="$2"
MOUNT_POINT=$(mktemp --directory)

# mount BSA file
"$EXECUTABLE" --archive "$BSA_FILE" "$MOUNT_POINT"
if [ $? -ne 0 ]
then
  echo "Could not mount BSA file on $MOUNT_POINT!"
  rmdir "$MOUNT_POINT"
  exit 1
fi

# Check existence of directories.
if [ ! -d "$MOUNT_POINT/some" ]
then
  echo "Directory 'some' does not exist in mounted directory!"
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi
if [ ! -d "$MOUNT_POINT/some/thing" ]
then
  echo "Directory 'some/thing' does not exist in mounted directory!"
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi
if [ ! -d "$MOUNT_POINT/something" ]
then
  echo "Directory 'something' does not exist in mounted directory!"
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi
if [ ! -d "$MOUNT_POINT/something/else" ]
then
  echo "Directory 'something/else' does not exist in mounted directory!"
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi
echo All expected directories exist.

# Check existence of files.
if [ ! -f "$MOUNT_POINT/some/thing/test.txt" ]
then
  echo "File 'some/thing/test.txt' does not exist in mounted directory!"
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi

if [ ! -f "$MOUNT_POINT/something/else/bar.txt" ]
then
  echo "File 'something/else/bar.txt' does not exist in mounted directory!"
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi

if [ ! -f "$MOUNT_POINT/something/else/foo.txt" ]
then
  echo "File 'something/else/foo.txt' does not exist in mounted directory!"
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi
echo All expected files exist.

# Check file contents.
CONTENT=$(cat "$MOUNT_POINT/some/thing/test.txt")
if [ "$CONTENT" != "This is a test." ]
then
  echo Content of some/thing/test.txt does not match expectation.
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi
CONTENT=$(cat "$MOUNT_POINT/something/else/bar.txt")
if [ "$CONTENT" != "foobar" ]
then
  echo Content of something/else/bar.txt does not match expectation.
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi
CONTENT=$(cat "$MOUNT_POINT/something/else/foo.txt")
if [ "$CONTENT" != "foo was here." ]
then
  echo Content of something/else/foo.txt does not match expectation.
  umount "$MOUNT_POINT"
  rmdir "$MOUNT_POINT"
  exit 1
fi
echo File contents match the expectations.

# Clean up.
umount "$MOUNT_POINT"
if [ $? -ne 0 ]
then
  echo "Could not un-mount BSA file from $MOUNT_POINT!"
  rmdir "$MOUNT_POINT"
  exit 1
fi

rmdir "$MOUNT_POINT"

exit 0
