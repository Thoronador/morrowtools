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
"%EXECUTABLE%" -d blah\foo\data -d blah\foo\data -p plugin.esp
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
"%EXECUTABLE%" -d x -p plugin.esp
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when data directory path was too short.
  exit /B 1
)

:: plugin file is given twice
"%EXECUTABLE%" --plugin-file plugin.esp --plugin-file plugin.esp
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when plugin was given twice.
  exit /B 1
)

:: no file name after parameter for plugin file
"%EXECUTABLE%" --plugin-file
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when --plugin-file was given without file name.
  exit /B 1
)

:: name of plugin is too short
"%EXECUTABLE%" --plugin-file a.b
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when plugin file name was too short.
  exit /B 1
)

:: no arguments given
"%EXECUTABLE%"
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when arguments were missing.
  exit /B 1
)

:: no plugin file given
"%EXECUTABLE%" -d blah\foo\bar
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when plugin file was missing.
  exit /B 1
)

:: Special Edition option is set twice
"%EXECUTABLE%" --special-edition --special-edition -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when Special Edition option was given twice.
  exit /B 1
)

:: old edition option is set twice
"%EXECUTABLE%" --original-edition --original-edition -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when Original Edition option was given twice.
  exit /B 1
)

exit 0
