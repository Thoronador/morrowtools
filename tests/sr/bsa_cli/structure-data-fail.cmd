:: Script to test executable when archive's structure data cannot be read.
::
::  Copyright (C) 2025  Dirk Stolle
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

:: 2nd parameter = path to test.bsa
if "%2" EQU "" (
  echo Second parameter must be path to BSA file!
  exit /B 1
)

SET BSA_FILE=%2

:: command check-hashes
"%EXECUTABLE%" check-hashes "%BSA_FILE%"
if %ERRORLEVEL% NEQ 2 (
  echo Executable command check-hashes did not exit with code 2 when BSA structure data was missing.
  exit /B 1
)

:: command directories
"%EXECUTABLE%" directories "%BSA_FILE%"
if %ERRORLEVEL% NEQ 2 (
  echo Executable command directories did not exit with code 2 when BSA structure data was missing.
  exit /B 1
)

:: command directory-metadata
"%EXECUTABLE%" directory-metadata "%BSA_FILE%"
if %ERRORLEVEL% NEQ 2 (
  echo Executable command directory-metadata did not exit with code 2 when BSA structure data was missing.
  exit /B 1
)

:: command extract-all
"%EXECUTABLE%" extract-all "%BSA_FILE%" "D:\foo\bar"
if %ERRORLEVEL% NEQ 2 (
  echo Executable command extract-all did not exit with code 2 when BSA structure data was missing.
  exit /B 1
)

:: command extract-directory
"%EXECUTABLE%" extract-directory "%BSA_FILE%" meshes\foo\bar "D:\foo\bar"
if %ERRORLEVEL% NEQ 2 (
  echo Executable command extract-directory did not exit with code 2 when BSA structure data was missing.
  exit /B 1
)

:: command extract-file
"%EXECUTABLE%" extract-file "%BSA_FILE%" meshes\foo\bar.dds "D:\foo\bar.dds"
if %ERRORLEVEL% NEQ 2 (
  echo Executable command extract-file did not exit with code 2 when BSA structure data was missing.
  exit /B 1
)

:: command file-metadata
"%EXECUTABLE%" file-metadata "%BSA_FILE%"
if %ERRORLEVEL% NEQ 2 (
  echo Executable command file-metadata did not exit with code 2 when BSA structure data was missing.
  exit /B 1
)

:: command list
"%EXECUTABLE%" list "%BSA_FILE%"
if %ERRORLEVEL% NEQ 2 (
  echo Executable command list did not exit with code 2 when BSA structure data was missing.
  exit /B 1
)

exit 0
