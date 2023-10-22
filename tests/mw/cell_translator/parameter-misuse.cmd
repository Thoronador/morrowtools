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

:: plugin to translate is given twice
"%EXECUTABLE%" -f file.esp -f plugin.esp
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when plugin file was given twice.
  exit /B 1
)

:: no file name after parameter for plugin file
"%EXECUTABLE%" -f
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when -f was given without file name.
  exit /B 1
)

:: output file is given twice
"%EXECUTABLE%" -o output.esp -o output.esp
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when output file was given twice.
  exit /B 1
)

:: no file name after parameter for output file
"%EXECUTABLE%" -o
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when -o was given without file name.
  exit /B 1
)

:: --force is given twice
"%EXECUTABLE%" --force --force
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when --force was given twice.
  exit /B 1
)
:: XML file is given twice
"%EXECUTABLE%" -xml data.xml -xml translation.xml
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when XML file was given twice.
  exit /B 1
)

:: no file name after parameter for XML file
"%EXECUTABLE%" -xml
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when -xml was given without file name.
  exit /B 1
)

:: --no-scripts is given twice
"%EXECUTABLE%" --no-scripts --no-scripts
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when --no-scripts was given twice.
  exit /B 1
)

:: --dare is given twice
"%EXECUTABLE%" --dare --dare
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when --dare was given twice.
  exit /B 1
)

:: -de is given twice
"%EXECUTABLE%" -de --deutsch
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when --deutsch was given twice.
  exit /B 1
)

:: -en is given twice
"%EXECUTABLE%" -en -en
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when -en was given twice.
  exit /B 1
)

:: no plugin file given
"%EXECUTABLE%" --english
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no plugin file was given.
  exit /B 1
)

:: plugin file and output file are identical
"%EXECUTABLE%" -f foo.esp -o foo.esp
if %ERRORLEVEL% NEQ 10 (
  echo Executable did not exit with code 10 plugin file and output file were identical.
  exit /B 1
)

:: output file is the Morrowind master file
"%EXECUTABLE%" -f foo.esp -o Morrowind.esm
if %ERRORLEVEL% NEQ 10 (
  echo Executable did not exit with code 10 when an invalid output file was given.
  exit /B 1
)

:: output file is the Tribunal master file
"%EXECUTABLE%" -f foo.esp -o Tribunal.esm
if %ERRORLEVEL% NEQ 10 (
  echo Executable did not exit with code 10 when an invalid output file, TR.esm, was given.
  exit /B 1
)

:: output file is the Bloodmoon master file
"%EXECUTABLE%" -f foo.esp -o Bloodmoon.esm
if %ERRORLEVEL% NEQ 10 (
  echo Executable did not exit with code 10 when an invalid output file, BM.esm, was given.
  exit /B 1
)

:: no arguments at all
"%EXECUTABLE%"
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when no arguments were given.
  exit /B 1
)

:: unknown / invalid parameter
"%EXECUTABLE%" --not-a-known-option
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when an unknown option was given.
  exit /B 1
)

exit 0
