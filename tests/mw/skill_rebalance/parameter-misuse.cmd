:: Script to test executable when parameters are used in the wrong way.
::
::  Copyright (C) 2023  Dirk Stolle
::
::  This program is free software: you can redistribute it and/or modify
::  it under the terms of the GNU Lesser General Public License as published by
::  the Free Software Foundation, either version 3 of the License, or
::  (at your option) any later version.
::
::  This program is distributed in the hope that it will be useful,
::  but WITHOUT ANY WARRANTY; without even the implied warranty of
::  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
::  GNU Lesser General Public License for more details.
::
::  You should have received a copy of the GNU Lesser General Public License
::  along with this program.  If not, see <http://www.gnu.org/licenses/>.

@echo off

:: 1st parameter = executable path
if "%1" EQU "" (
  echo First parameter must be executable file!
  exit /B 1
)

SET EXECUTABLE=%1

:: path to data files is given twice
"%EXECUTABLE%" -d blah\foo\data -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when data directory was given twice.
  exit /B 1
)

:: no path after parameter for data files directory
"%EXECUTABLE%" -d
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when -d was given without directory path.
  exit /B 1
)

:: path to data files is too short
"%EXECUTABLE%" -d x
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when data directory path was too short.
  exit /B 1
)

:: no file name after parameter for output file
"%EXECUTABLE%" -o
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when -o was given without file name.
  exit /B 1
)

:: no float value after parameter for scaling factor
"%EXECUTABLE%" -s
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when -s was given without scaling value.
  exit /B 1
)

:: invalid value after parameter for scaling factor
"%EXECUTABLE%" -s foo
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when -s was given a non-float value.
  exit /B 1
)

:: scaling factor too large / too small + invalid parameter
"%EXECUTABLE%" -s -1.05 -s 123456 --foo-bar-fail
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when invalid parameters were given.
  exit /B 1
)

:: no scaling factor / no arguments at all
"%EXECUTABLE%"
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no scaling factor was given.
  exit /B 1
)

:: unknown / invalid parameter
"%EXECUTABLE%" --not-a-known-option
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when an unknown option was given twice.
  exit /B 1
)

exit 0
