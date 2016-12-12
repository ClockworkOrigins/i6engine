@echo OFF

REM
REM i6engine
REM Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
REM 
REM This file is part of i6engine; i6engine is free software; you can redistribute it and/or
REM modify it under the terms of the GNU Lesser General Public
REM License as published by the Free Software Foundation; either
REM version 2.1 of the License, or (at your option) any later version.
REM 
REM This library is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
REM Lesser General Public License for more details.
REM 
REM You should have received a copy of the GNU Lesser General Public
REM License along with this library; if not, write to the Free Software
REM Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
REM

Set ROOT_DIR=%3
Set COPY_DIR=%4
Set CONFIG_DIR=%5

call build-common.bat %1 %2

SET SOURCE_DIR=%cd%
SET DEP_DIR=%ROOT_DIR%\..
Set ARCHIVE=m2etis-0.4.0-src-rev121.zip
Set BUILD_DIR=%TMP_DIR%/m2etis-0.4.0-src-rev121
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/m2etis

IF EXIST %PREFIX% EXIT /B

echo "Compile m2etis"

echo "Extracting m2etis"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR% http://clockwork-origins.de/m2etis/downloads/

echo "Configuring m2etis"

cd %BUILD_DIR%
IF EXIST %COPY_DIR% (
	xcopy /I /S /Y "%COPY_DIR%" "%BUILD_DIR%\library\extern\"
)
IF EXIST "%DEP_DIR%\..\config\m2etis" (
	xcopy /I /S /Y "%CONFIG_DIR%" "%BUILD_DIR%\library\generated\"
)
cmake -DWITH_TESTING=OFF -DWITH_SIM=OFF -DWITH_LOGGING=OFF  -DWITH_MESSAGECOMPRESSION=OFF  -DM2ETIS_BUILD_SHARED=ON -DADDITIONAL_INCLUDE_DIRECTORIES=%BUILD_DIR%\library\extern -DDEP_DIR=%ROOT_DIR%/ -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%VSCOMPILER%%VSARCH%" .

echo "Building m2etis"

MSBuild.exe m2etis.sln /m:%NUMBER_OF_PROCESSORS% /p:Configuration=Release

echo "Installing m2etis"

MSBuild.exe INSTALL.vcxproj /p:Configuration=Release

echo "Cleaning up"

cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%/.."

