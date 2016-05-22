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

call build-common.bat %1 %2

call build-boost.bat %1 %2
call build-bullet.bat %1 %2
REM depends on boost
call build-ogre.bat %1 %2
REM depends on ogre and boost
call build-cegui.bat %1 %2
call build-gperftools.bat %1 %2
call build-gmock.bat %1 %2
call build-tinyxml.bat %1 %2
call build-clockUtils.bat %1 %2
call build-openalsoft.bat %1 %2
call build-wxWidgets.bat %1 %2
REM depends on boost
REM call build-m2etis.bat

