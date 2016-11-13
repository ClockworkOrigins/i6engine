##
# i6engine
# Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
# 
# This file is part of i6engine; i6engine is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
##

SET(CMAKE_LIBRARY_PATH ${I6ENGINE_DEP_DIR})

#----------------------------------------------------
# Boost
#----------------------------------------------------

IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/boost/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-boost.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/boost/")
IF(CMAKE_HOST_WIN32 AND ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/boost/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-boost-android.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(CMAKE_HOST_WIN32 AND ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/boost/")
IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/boost/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-boost.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/boost/")
IF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32 AND NOT EXISTS "${I6ENGINE_DEP_DIR}/boost/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-boost-android.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32 AND NOT EXISTS "${I6ENGINE_DEP_DIR}/boost/")
SET(LIBNAME "BOOST")
SET(LIBHEADER "boost/thread.hpp")
SET(BOOST_ROOT ${I6ENGINE_DEP_DIR}/boost)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_atomic)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_date_time)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_filesystem)
IF(ISIXE_WITH_LOGGING)
	SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_log)
ENDIF(ISIXE_WITH_LOGGING)
IF(SCRIPTING_PYTHON)
	SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_python)
ENDIF(SCRIPTING_PYTHON)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_regex)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_serialization)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_system)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_thread)
find_package(EasyFind REQUIRED COMPONENTS ${BOOST_COMPONENT})
include_directories(SYSTEM ${BOOST_INCLUDE_DIR})

#----------------------------------------------------
# Bullet
#----------------------------------------------------

IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/bullet/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-bullet.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/bullet/")
IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/bullet/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-bullet.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/bullet/")
SET(LIBNAME "BULLET")
SET(LIBHEADER "bullet/btBulletCollisionCommon.h")
SET(BULLET_ROOT ${I6ENGINE_DEP_DIR}/bullet)
find_package(EasyFind REQUIRED COMPONENTS BulletCollision BulletDynamics BulletFileLoader BulletWorldImporter LinearMath)
include_directories(SYSTEM ${BULLET_INCLUDE_DIR})
include_directories(SYSTEM ${BULLET_INCLUDE_DIR}/bullet)

#----------------------------------------------------
# clockUtils
#----------------------------------------------------

IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/clockUtils/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-clockUtils.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/clockUtils/")
IF(CMAKE_HOST_WIN32 AND ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/clockUtils/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-clockUtils-android.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(CMAKE_HOST_WIN32 AND ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/clockUtils/")
IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/clockUtils/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-clockUtils.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/clockUtils/")
IF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32 AND NOT EXISTS "${I6ENGINE_DEP_DIR}/clockUtils/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-clockUtils-android.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32 AND NOT EXISTS "${I6ENGINE_DEP_DIR}/clockUtils/")
SET(LIBNAME "CLOCKUTILS")
SET(LIBHEADER "clockUtils/errors.h")
SET(CLOCKUTILS_ROOT ${I6ENGINE_DEP_DIR}/clockUtils)
SET(CLOCKUTILS_COMPONENT ${CLOCKUTILS_COMPONENT} clock_argParser)
SET(CLOCKUTILS_COMPONENT ${CLOCKUTILS_COMPONENT} clock_compression)
SET(CLOCKUTILS_COMPONENT ${CLOCKUTILS_COMPONENT} clock_iniParser)
SET(CLOCKUTILS_COMPONENT ${CLOCKUTILS_COMPONENT} clock_sockets)

find_package(EasyFind REQUIRED COMPONENTS ${CLOCKUTILS_COMPONENT})
include_directories(SYSTEM ${CLOCKUTILS_INCLUDE_DIR})

#----------------------------------------------------
# Doxygen
#----------------------------------------------------

IF(ISIXE_WITH_DOCU)
	find_package(Doxygen)
ENDIF(ISIXE_WITH_DOCU)

#----------------------------------------------------
# GMock
#----------------------------------------------------

IF(ISIXE_WITH_TESTING)
	IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/gmock/")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-gmock.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/gmock/")
	IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/gmock/")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-gmock.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/gmock/")
	SET(LIBNAME "GTEST")
	SET(LIBHEADER "gtest/gtest.h")
	SET(GTEST_ROOT ${I6ENGINE_DEP_DIR}/gmock)
	SET(GTEST_COMPONENT ${GTEST_COMPONENT} gmock)
	SET(GTEST_COMPONENT ${GTEST_COMPONENT} gtest)
	SET(GTEST_COMPONENT ${GTEST_COMPONENT} gtest_main)

	find_package(EasyFind REQUIRED COMPONENTS ${GTEST_COMPONENT})
	include_directories(SYSTEM ${GTEST_INCLUDE_DIR})
ENDIF(ISIXE_WITH_TESTING)

#----------------------------------------------------
# JPeg
#----------------------------------------------------

IF(UNIX)
	find_package(JPEG REQUIRED)
	include_directories(SYSTEM ${JPEG_INCLUDE_DIR})
ENDIF(UNIX)

#----------------------------------------------------
# Lua
#----------------------------------------------------

IF(SCRIPTING_LUA)
	IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/lua/")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-lua.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/lua/")
	IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/lua/")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-lua.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/lua/")
	SET(LIBNAME "LUA")
	SET(LIBHEADER "lua.h")
	SET(LUA_ROOT ${I6ENGINE_DEP_DIR}/lua)
	find_package(EasyFind REQUIRED COMPONENTS lua)
	include_directories(SYSTEM ${LUA_INCLUDE_DIR})
ENDIF(SCRIPTING_LUA)

#----------------------------------------------------
# m2etis
#----------------------------------------------------

IF(ISIXE_WITH_NETWORK)
	IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/m2etis/")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-m2etis.bat ${VS_TOOLCHAIN} ${VS_ARCH} "${I6ENGINE_DEP_DIR}" "${CMAKE_BINARY_DIR}/m2etisIncludes" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/m2etis/")
	IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/m2etis/")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-m2etis.sh ${UNIX_COMPILER}  "${I6ENGINE_DEP_DIR}" "${CMAKE_BINARY_DIR}/m2etisIncludes" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/m2etis/")
	SET(LIBNAME "M2ETIS")
	SET(LIBHEADER "m2etis/pubsub/PubSubSystem.h")
	SET(M2ETIS_ROOT ${I6ENGINE_DEP_DIR}/m2etis)
	find_package(EasyFind REQUIRED COMPONENTS m2etis)
	include_directories(SYSTEM ${M2ETIS_INCLUDE_DIR})
ENDIF(ISIXE_WITH_NETWORK)

#----------------------------------------------------
# Ogre
#----------------------------------------------------

IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/ogre/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-ogre.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/ogre/")
IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/ogre/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-ogre.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/ogre/")
SET(LIBNAME "OIS")
SET(LIBHEADER "OIS/OIS.h")
SET(OIS_ROOT ${I6ENGINE_DEP_DIR}/misc)
find_package(EasyFind REQUIRED COMPONENTS OIS)
include_directories(SYSTEM ${OIS_INCLUDE_DIR})
SET(LIBNAME "OGRE")
SET(LIBHEADER "OGRE/Ogre.h")
SET(OGRE_ROOT ${I6ENGINE_DEP_DIR}/ogre)
find_package(EasyFind REQUIRED COMPONENTS OgreMain OgreOverlay OgrePaging OgreProperty OgreRTShaderSystem OgreTerrain)
include_directories(SYSTEM ${OGRE_INCLUDE_DIR})
include_directories(SYSTEM ${OGRE_INCLUDE_DIR}/OGRE)
include_directories(SYSTEM ${OGRE_INCLUDE_DIR}/OGRE/Paging)
include_directories(SYSTEM ${OGRE_INCLUDE_DIR}/OGRE/Terrain)

#----------------------------------------------------
# OpenAL Soft
#----------------------------------------------------

IF(ISIXE_WITH_AUDIO)
	IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/OpenALSoft/")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-openalsoft.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/OpenALSoft/")
	IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/OpenALSoft/")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-openalsoft.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/OpenALSoft/")
	SET(LIBNAME "OPENALSOFT")
	SET(LIBHEADER "AL/al.h")
	SET(OPENALSOFT_ROOT ${I6ENGINE_DEP_DIR}/OpenALSoft)
	IF(WIN32)
		SET(OPENALSOFT_COMPONENT ${OPENALSOFT_COMPONENT} OpenAL32)
	ELSEIF(UNIX)
		SET(OPENALSOFT_COMPONENT ${OPENALSOFT_COMPONENT} openal)
	ENDIF()

	find_package(EasyFind REQUIRED COMPONENTS ${OPENALSOFT_COMPONENT})
	include_directories(SYSTEM ${OPENALSOFT_INCLUDE_DIR})
ENDIF(ISIXE_WITH_AUDIO)

#----------------------------------------------------
# PThread
#----------------------------------------------------

IF(UNIX)
	find_package(Threads REQUIRED)
	include_directories(SYSTEM ${PTHREADS_INCLUDE_DIR})
ENDIF(UNIX)

#----------------------------------------------------
# Python
#----------------------------------------------------

IF(SCRIPTING_PYTHON)
	find_package(PythonLibs 2.7 REQUIRED)
	include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS})
ENDIF(SCRIPTING_PYTHON)
#----------------------------------------------------
# Qt
#----------------------------------------------------

IF(ISIXE_WITH_TOOLS)
	IF(UNIX AND NOT ANDROID)
		SET(ENV{Qt5_DIR} "")
	ENDIF()

	SET(QT_WEBENGINE_VERSION 5.6.0)

	# Test for supported Qt version
	find_program(QMAKE_EXECUTABLE NAMES qmake HINTS $ENV{Qt5_DIR} ENV QTDIR PATH_SUFFIXES bin)
	execute_process(COMMAND ${QMAKE_EXECUTABLE} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION)

	SET(QT_SEARCH_MODULES "")

	IF(QT_VERSION LESS QT_WEBENGINE_VERSION)
		SET(QT_SEARCH_MODULES ${QT_SEARCH_MODULES} Core)
		SET(QT_SEARCH_MODULES ${QT_SEARCH_MODULES} Gui)
		SET(QT_SEARCH_MODULES ${QT_SEARCH_MODULES} LinguistTools)
		SET(QT_SEARCH_MODULES ${QT_SEARCH_MODULES} Widgets)
	ELSE()
		SET(QT_SEARCH_MODULES ${QT_SEARCH_MODULES} Core)
		SET(QT_SEARCH_MODULES ${QT_SEARCH_MODULES} Gui)
		SET(QT_SEARCH_MODULES ${QT_SEARCH_MODULES} LinguistTools)
		SET(QT_SEARCH_MODULES ${QT_SEARCH_MODULES} Widgets)
	ENDIF()

	FIND_PACKAGE(Qt5 REQUIRED ${QT_SEARCH_MODULES} HINTS $ENV{Qt5_DIR})

	IF(QT_VERSION LESS QT_WEBENGINE_VERSION)
		INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
		SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Core_LIBRARIES})
		INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
		SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Gui_LIBRARIES})
		INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
		SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES})
	ELSE()
		INCLUDE_DIRECTORIES(SYSTEM ${Qt5Core_INCLUDE_DIRS})
		SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Core_LIBRARIES})
		INCLUDE_DIRECTORIES(SYSTEM ${Qt5Gui_INCLUDE_DIRS})
		SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Gui_LIBRARIES})
		INCLUDE_DIRECTORIES(SYSTEM ${Qt5Widgets_INCLUDE_DIRS})
		SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES})
	ENDIF()
ENDIF(ISIXE_WITH_TOOLS)


#----------------------------------------------------
# tinyxml
#----------------------------------------------------

IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/tinyxml2/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-tinyxml.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/tinyxml2/")
IF(CMAKE_HOST_WIN32 AND ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/tinyxml2/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-tinyxml-android.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(CMAKE_HOST_WIN32 AND ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/tinyxml2/")
IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/tinyxml2/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-tinyxml.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/tinyxml2/")
IF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32 AND NOT EXISTS "${I6ENGINE_DEP_DIR}/tinyxml2/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-tinyxml-android.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32 AND NOT EXISTS "${I6ENGINE_DEP_DIR}/tinyxml2/")
SET(LIBNAME "TINYXML")
SET(LIBHEADER "tinyxml2.h")
SET(TINYXML_ROOT ${I6ENGINE_DEP_DIR}/tinyxml2)
find_package(EasyFind REQUIRED COMPONENTS tinyxml2)
include_directories(SYSTEM ${TINYXML_INCLUDE_DIR})

#----------------------------------------------------
# CEGUI
#----------------------------------------------------

IF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/cegui/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-cegui.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(WIN32 AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/cegui/")
IF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/cegui/")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-cegui.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND NOT ANDROID AND NOT EXISTS "${I6ENGINE_DEP_DIR}/cegui/")
SET(LIBNAME "CEGUI")
SET(LIBHEADER "CEGUI/CEGUI.h")
SET(CEGUI_ROOT ${I6ENGINE_DEP_DIR}/cegui)
find_package(EasyFind REQUIRED COMPONENTS CEGUIBase-0 CEGUIExpatParser CEGUIOgreRenderer-0)
include_directories(SYSTEM ${CEGUI_INCLUDE_DIR})
