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

call build-common.bat android

Set ARCHIVE=boost_1_58_0.tar.bz2
Set BUILD_DIR=%TMP_DIR%/boost_1_58_0
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/boost

IF EXIST %PREFIX% EXIT /B

echo "Compile Boost"

echo "Extracting Boost"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Configuring Boost"

cd %BUILD_DIR%
xcopy /F "%DEP_DIR%\..\ext\patches\boost\user-config-AndroidWindows.jam" "%BUILD_DIR%\tools\build\src\user-config.jam*"
call bootstrap.bat

echo "Building Boost"
b2 toolset=gcc-android target-os=linux --with-atomic --with-date_time --with-filesystem --with-log --with-regex --with-serialization --with-system --with-thread link=shared threading=multi --layout=system -j %NUMBER_OF_PROCESSORS% variant=release install --prefix=%PREFIX% stage

echo "Installing Boost"

echo #define BOOST_ALL_NO_LIB >> "%PREFIX%\include\boost\config\user.hpp"
echo #define BOOST_ALL_DYN_LINK >> "%PREFIX%\include\boost\config\user.hpp"

echo "Cleaning up"

cd %DEP_DIR%
RD /S /Q "%TMP_DIR%"

