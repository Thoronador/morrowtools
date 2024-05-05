:: Script to test executable when a parameter contains umlauts.
::
::  Copyright (C) 2024  Dirk Stolle
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

:: 2nd parameter = path to Möwe.bsa
if "%2" EQU "" (
  echo Second parameter must be path to BSA file!
  exit /B 1
)

SET BSA_FILE=%2

:: check-hashes
"%EXECUTABLE%" check-hashes "%BSA_FILE%"
if %ERRORLEVEL% NEQ 0 (
  echo Executable did not exit with code 0 when check-hashes operation was executed.
  exit /B 1
)

:: directories
"%EXECUTABLE%" directories "%BSA_FILE%"
if %ERRORLEVEL% NEQ 0 (
  echo Executable did not exit with code 0 when directories operation was executed.
  exit /B 1
)

:: directory-metadata
"%EXECUTABLE%" directory-metadata "%BSA_FILE%"
if %ERRORLEVEL% NEQ 0 (
  echo Executable did not exit with code 0 when directory-metadata operation was executed.
  exit /B 1
)

:: file-metadata
"%EXECUTABLE%" file-metadata "%BSA_FILE%"
if %ERRORLEVEL% NEQ 0 (
  echo Executable did not exit with code 0 when file-metadata operation was executed.
  exit /B 1
)

:: info
"%EXECUTABLE%" info "%BSA_FILE%"
if %ERRORLEVEL% NEQ 0 (
  echo Executable did not exit with code 0 when info operation was executed.
  exit /B 1
)

:: list
"%EXECUTABLE%" list "%BSA_FILE%"
if %ERRORLEVEL% NEQ 0 (
  echo Executable did not exit with code 0 when list operation was executed.
  exit /B 1
)

exit 0
