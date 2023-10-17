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

:: invalid operation given
"%EXECUTABLE%" not-an-operation file.bsa
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when an invalid operation was set.
  exit /B 1
)

:: no arguments given
"%EXECUTABLE%"
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when arguments were missing.
  exit /B 1
)

:: BSA file does not exist / fails to load
"%EXECUTABLE%" info this-file-is-missing.bsa
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when BSA file was missing.
  exit /B 1
)

exit 0
