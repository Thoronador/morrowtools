:: Script to test executable's exit code to indicate its own version.
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

:: non-zero exitcode does not indicate error but the currently used version
"%EXECUTABLE%" --version-with-exitcode
SET CODE=%ERRORLEVEL%

echo Version exit code is %CODE%.

if %CODE% EQU 1 (
  echo Executable did exit with code 1 when version was requested.
  exit /B 1
)

if %CODE% EQU 0 (
  echo Executable did exit with code 0 when version was requested.
  exit /B 1
)

exit 0
