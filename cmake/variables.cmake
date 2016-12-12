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

IF(ANDROID)
	ADD_DEFINITIONS(-DANDROID)
	SET(ANDROID_STL "gnustl_shared")
	SET(DEP_DIR_BUILD android)
	SET(ANDROID_SDK $ENV{ANDROID_HOME} CACHE PATH "Path to ANDROID SDK")
		IF(NOT M2ETIS_CONFIG_DIR)
			SET(M2ETIS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config/m2etis)
		ENDIF(NOT M2ETIS_CONFIG_DIR)
		IF(NOT M2ETIS_CONFIG_DIR)
			SET(M2ETIS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config/m2etis)
		ENDIF(NOT M2ETIS_CONFIG_DIR)
	IF(EXISTS $ENV{Qt5_BaseDir}/android_armv7/)
		SET(Qt5_Platform android_armv7)
	ELSE()
		SET(Qt5_Platform android_armv7a)
	ENDIF()
		SET(ENV{Qt5_DIR} $ENV{Qt5_BaseDir}/${Qt5_Platform})
		SET(Qt5_libDir "$ENV{Qt5_DIR}/lib" CACHE STRING "Directory containing the qt libs")
	IF(EXISTS $ENV{Qt5_BaseDir}/android_armv7/)
		SET(Qt5_Platform android_armv7)
	ELSE()
		SET(Qt5_Platform android_armv7a)
	ENDIF()
		SET(ENV{Qt5_DIR} $ENV{Qt5_BaseDir}/${Qt5_Platform})
		SET(Qt5_libDir "$ENV{Qt5_DIR}/lib" CACHE STRING "Directory containing the qt libs")
ELSEIF(UNIX)
	IF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
		SET(DEP_DIR_BUILD "gcc")
		SET(UNIX_COMPILER "gcc")
		IF(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "4.8")
			SET(UNIX_COMPILER "gcc-4.7")
			SET(DEP_DIR_BUILD "gcc-4.7")
		ENDIF()
		IF(NOT M2ETIS_CONFIG_DIR)
			SET(M2ETIS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config/m2etis)
		ENDIF(NOT M2ETIS_CONFIG_DIR)
		SET(Qt5_Platform gcc_64)
		SET(ENV{Qt5_DIR} $ENV{Qt5_BaseDir}/${Qt5_Platform})
		SET(Qt5_libDir "$ENV{Qt5_DIR}/lib" CACHE STRING "Directory containing the qt libs")
	ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
		SET(DEP_DIR_BUILD "clang")
		SET(UNIX_COMPILER "clang")
		IF(NOT M2ETIS_CONFIG_DIR)
			SET(M2ETIS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config/m2etis)
		ENDIF(NOT M2ETIS_CONFIG_DIR)
		SET(ENV{Qt5_DIR} $ENV{Qt5_BaseDir}/${Qt5_Platform})
		SET(Qt5_libDir "$ENV{Qt5_DIR}/lib" CACHE STRING "Directory containing the qt libs")
	ENDIF()
ELSEIF(WIN32)
	IF(CMAKE_GENERATOR MATCHES "Visual Studio 12 2013 Win64")
		SET(DEP_DIR_BUILD "msvc13_64")
		SET(VS_TOOLCHAIN "msvc13")
		SET(VSENV VS12)
		SET(VS_ARCH "64")
		SET(VSSCRIPTARCH "amd64")
		IF(NOT M2ETIS_CONFIG_DIR)
			SET(M2ETIS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config/m2etis)
		ENDIF(NOT M2ETIS_CONFIG_DIR)
		SET(Qt5_Platform msvc2013_64)
		SET(ENV{Qt5_DIR} $ENV{Qt5_BaseDir}/${Qt5_Platform})
		SET(Qt5_libDir "$ENV{Qt5_DIR}/lib" CACHE STRING "Directory containing the qt libs")
	ELSEIF(CMAKE_GENERATOR MATCHES "Visual Studio 12 2013")
		SET(DEP_DIR_BUILD "msvc13_32")
		SET(VS_TOOLCHAIN "msvc13")
		SET(VSENV VS12)
		SET(VS_ARCH "32")
		SET(VSSCRIPTARCH "x86")
		IF(NOT M2ETIS_CONFIG_DIR)
			SET(M2ETIS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config/m2etis)
		ENDIF(NOT M2ETIS_CONFIG_DIR)
		SET(Qt5_Platform msvc2013)
		SET(ENV{Qt5_DIR} $ENV{Qt5_BaseDir}/${Qt5_Platform})
		SET(Qt5_libDir "$ENV{Qt5_DIR}/lib" CACHE STRING "Directory containing the qt libs")
	ELSEIF(CMAKE_GENERATOR MATCHES "Visual Studio 14 2015 Win64")
		SET(DEP_DIR_BUILD "msvc15_64")
		SET(VS_TOOLCHAIN "msvc15")
		SET(VSENV VS15)
		SET(VS_ARCH "64")
		SET(VSSCRIPTARCH "amd64")
		IF(NOT M2ETIS_CONFIG_DIR)
			SET(M2ETIS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config/m2etis)
		ENDIF(NOT M2ETIS_CONFIG_DIR)
		SET(Qt5_Platform msvc2015_64)
		SET(ENV{Qt5_DIR} $ENV{Qt5_BaseDir}/${Qt5_Platform})
		SET(Qt5_libDir "$ENV{Qt5_DIR}/lib" CACHE STRING "Directory containing the qt libs")
	ELSEIF(CMAKE_GENERATOR MATCHES "Visual Studio 14 2015")
		SET(DEP_DIR_BUILD "msvc15_32")
		SET(VS_TOOLCHAIN "msvc15")
		SET(VSENV VS15)
		SET(VS_ARCH "32")
		SET(VSSCRIPTARCH "x86")
		IF(NOT M2ETIS_CONFIG_DIR)
			SET(M2ETIS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config/m2etis)
		ENDIF(NOT M2ETIS_CONFIG_DIR)
		SET(Qt5_Platform msvc2015)
		SET(ENV{Qt5_DIR} $ENV{Qt5_BaseDir}/${Qt5_Platform})
		SET(Qt5_libDir "$ENV{Qt5_DIR}/lib" CACHE STRING "Directory containing the qt libs")
	ENDIF()
ENDIF()

SET(I6ENGINE_DEP_DIR "${CMAKE_SOURCE_DIR}/dependencies/${DEP_DIR_BUILD}" CACHE STRING "path to deps for i6engine")

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#########################################################################
# Global Macros and Definitions
#########################################################################

# Sets appropriate Compilerflags

SET(CXX_FLAGS "-pedantic -Wall -Wextra -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wredundant-decls -Wctor-dtor-privacy -Wno-sign-conversion -Wno-unused-parameter -Wno-long-long -fPIC -std=c++11")
SET(CLANG_FLAGS "-Wstring-plus-int")

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	IF(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7)
		SET(CXX_FLAGS "${CXX_FLAGS} -Wold-style-cast")
	ENDIF()
	IF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
		SET(CXX_FLAGS "${CXX_FLAGS} -Wno-old-style-cast")
	ENDIF()
	SET(CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} -g")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} -0s -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} -O3 -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	SET(CXX_FLAGS "${CXX_FLAGS} -Wold-style-cast")
	SET(CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} ${CLANG_FLAGS} -g")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} ${CLANG_FLAGS} -0s -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} ${CLANG_FLAGS} -O3 -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} ${CLANG_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	SET(CXX_FLAGS "/MP /W4 /wd4127 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /D_WINSOCK_DEPRECATED_NO_WARNINGS")
	SET(CMAKE_CXX_FLAGS_DEBUG		"${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL		"${CMAKE_CXX_FLAGS_MINSIZEREL} ${CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELEASE		"${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CXX_FLAGS}")
ELSE()
	MESSAGE(SEND_FATAL "Unknown C++ compiler \"${CMAKE_CXX_COMPILER_ID}\".")
ENDIF()

#########################################################################
# Global Directory Definitions
#########################################################################

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
