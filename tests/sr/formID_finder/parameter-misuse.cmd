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

:: search keyword is given twice
"%EXECUTABLE%" --keyword bow --keyword -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when keyword was given twice.
  exit /B 1
)

:: no keyword after parameter for search keyword
"%EXECUTABLE%" --keyword
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when --keyword was given without actual keyword.
  exit /B 1
)

:: case-sensitivity is set twice
"%EXECUTABLE%" --case-sensitive --case-sensitive -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when case-sensitivity was given twice.
  exit /B 1
)

:: quest information option is set twice
"%EXECUTABLE%" --all-quest-info --all-quest-info -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when quest info option was given twice.
  exit /B 1
)

:: faction rank option is set twice
"%EXECUTABLE%" --faction-ranks --faction-ranks -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when faction ranks option was given twice.
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

:: English language is set twice
"%EXECUTABLE%" --en --en -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when English language was given twice.
  exit /B 1
)

:: French language is set twice
"%EXECUTABLE%" --fr --french -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when French language was given twice.
  exit /B 1
)

:: German language is set twice
"%EXECUTABLE%" --de --german -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when German language was given twice.
  exit /B 1
)

:: Italian language is set twice
"%EXECUTABLE%" --it --italian -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when Italian language was given twice.
  exit /B 1
)

:: Polish language is set twice
"%EXECUTABLE%" --pl --polish -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when Polish language was given twice.
  exit /B 1
)

:: Russian language is set twice
"%EXECUTABLE%" --ru --russian -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when Russian language was given twice.
  exit /B 1
)

:: Spanish language is set twice
"%EXECUTABLE%" --es --spanish -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when Spanish language was given twice.
  exit /B 1
)

:: reference ids are requested twice
"%EXECUTABLE%" --ref-id --ref-id -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when reference ids were requested twice.
  exit /B 1
)

:: file names are requested twice
"%EXECUTABLE%" --show-files --show-files  -d blah\foo\data
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when file names were requested twice.
  exit /B 1
)

:: no arguments given
"%EXECUTABLE%"
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when arguments were missing.
  exit /B 1
)

:: no search keyword given
"%EXECUTABLE%" --ref-id
if %ERRORLEVEL% NEQ 1 (
  echo Executable did not exit with code 1 when search keyword was missing.
  exit /B 1
)

exit 0
