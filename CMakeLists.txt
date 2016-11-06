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

# CMake main configfile i6engine project
#

cmake_minimum_required (VERSION 3.0)

#########################################################################
#########################################################################
#
# Global Build Configuration Options
#
#########################################################################

option(ISIXE_WITH_LOGGING "enables Logging" OFF)
option(ISIXE_WITH_NETWORK "build i6engine with networking capabilities" OFF)
option(ISIXE_WITH_PROFILING "build i6engine with profiling" OFF)
option(ISIXE_WITH_TESTING "build i6engine with tests" OFF)
option(ISIXE_WITH_AUDIO "build i6engine with OpenAL Soft audio subsystem" ON)
option(ISIXE_WITH_EDITOR "build i6engine with the level editor" ON)
option(ISIXE_WITH_SAMPLES "build some samples for i6engine" ON)
option(ISIXE_WITH_TOOLS "build some tools for i6engine" ON)
option(ISIXE_WITH_RPG "build rpg module for i6engine" ON)
option(ISIXE_WITH_DOCU "build documentation for i6engine" ON)
option(ISIXE_WITH_TUTORIALS "build tutorials for i6engine" ON)
OPTION(ISIXE_WITH_CONSOLE "build samples and tools with console output" ON)
OPTION(ISIXE_WITH_PARTICLEUNIVERSE "build with ParticleUniverse particle system. If disabled, no particles are available" ON)
set(ISIXE_WITH_SCRIPTING "python" CACHE STRING "specifies the scripting system to use" )
# options are
# * ""
# * "python"
# * "lua"

IF(NOT ISIXE_WITH_CONSOLE AND WIN32)
	SET(GUI_TYPE WIN32)
ENDIF(NOT ISIXE_WITH_CONSOLE AND WIN32)

IF(ISIXE_WITH_TESTING AND NOT WIN32)
	# static for gmock
	option(ISIXE_BUILD_SHARED "build project with shared libraries" OFF)
ELSE(ISIXE_WITH_TESTING AND NOT WIN32)
	option(ISIXE_BUILD_SHARED "build project with shared libraries" ON)
ENDIF(ISIXE_WITH_TESTING AND NOT WIN32)

project(i6engine)

include(cmake/variables.cmake)

IF(ISIXE_WITH_SCRIPTING STREQUAL "lua")
	set(SCRIPTING_LUA ON)
	set(SCRIPTING_PYTHON OFF)
	set(ISIXE_SCRIPTING "SCRIPTING_LUA")
	SET(SCRIPTING_NAME "Lua")
ELSEIF(ISIXE_WITH_SCRIPTING STREQUAL "python")
	set(SCRIPTING_PYTHON ON)
	set(SCRIPTING_LUA OFF)
	set(ISIXE_SCRIPTING "SCRIPTING_PYTHON")
	SET(SCRIPTING_NAME "Python")
ELSE()
	set(SCRIPTING_LUA OFF)
	set(SCRIPTING_PYTHON OFF)
	set(ISIXE_SCRIPTING "SCRIPTING_NONE")
ENDIF()

# Setting last version
SET(LAST_VERSION_MAJOR 1)
SET(LAST_VERSION_MINOR 0)
SET(LAST_VERSION_PATCH 1)

# Setting current version
SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 0)

###############################################################################
# Configure Build Settings (has to be before building m2etis because m2etis needs i6engineBuildSettings)
###############################################################################

configure_file(${CMAKE_SOURCE_DIR}/config/i6engineBuildSettings.h.in ${CMAKE_BINARY_DIR}/include/i6engine/i6engineBuildSettings.h @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/include/i6engine/i6engineBuildSettings.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/i6engine)

configure_file(${CMAKE_SOURCE_DIR}/config/i6engineBuildSettings.h.in ${CMAKE_BINARY_DIR}/m2etisIncludes/i6engine/i6engineBuildSettings.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/i6engine-utils/include/i6engine/utils/i6eSystemParameters.h ${CMAKE_BINARY_DIR}/m2etisIncludes/i6engine/utils/i6eSystemParameters.h COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/i6engine-core/include/i6engine/core/messaging/IPKey.h ${CMAKE_BINARY_DIR}/m2etisIncludes/i6engine/core/messaging/IPKey.h COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/i6engine-core/include/i6engine/core/messaging/Message.h ${CMAKE_BINARY_DIR}/m2etisIncludes/i6engine/core/messaging/Message.h COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/i6engine-core/include/i6engine/core/messaging/MessageStruct.h ${CMAKE_BINARY_DIR}/m2etisIncludes/i6engine/core/messaging/MessageStruct.h COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/i6engine-modules/include/i6engine/api/GameMessage.h ${CMAKE_BINARY_DIR}/m2etisIncludes/i6engine/api/GameMessage.h COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/i6engine-modules/include/i6engine/api/GameMessageStruct.h ${CMAKE_BINARY_DIR}/m2etisIncludes/i6engine/api/GameMessageStruct.h COPYONLY)

#########################################################################
# Library Section
#########################################################################

include(cmake/searchDependencies.cmake)

#----------------------------------------------------
# Python
#----------------------------------------------------
IF(SCRIPTING_PYTHON)
	IF(WIN32)
		SET(PYTHON_DLL "$ENV{windir}")
		IF(64BIT)
			SET(PYTHON_DLL "${PYTHON_DLL}\\System32")
		ELSE(64BIT)
			SET(PYTHON_DLL "${PYTHON_DLL}\\SysWOW64")
		ENDIF(64BIT)
		SET(PYTHON_DLL "${PYTHON_DLL}\\python27.dll")
	ENDIF(WIN32)
ENDIF()

#----------------------------------------------------
# Qt
#----------------------------------------------------
IF(ISIXE_WITH_TOOLS)
	IF(ISIXE_WITH_CONSOLE AND WIN32)
		SET(QT_LIBRARIES ${QT_LIBRARIES} Qt5::WinMain)
	ENDIF(ISIXE_WITH_CONSOLE AND WIN32)

	# Translation files
	SET(TRANS
			 ${CMAKE_SOURCE_DIR}/languages/de_DE.ts
			 ${CMAKE_SOURCE_DIR}/languages/en_US.ts
	)

	# used for Windows prebuilt packages
	SET(QT_EXPORT_DIR $ENV{Qt5_DIR})
ENDIF(ISIXE_WITH_TOOLS)

####################################################################################
# Install paths
####################################################################################

set(LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" CACHE STRING "Library directory name")
## the following are directories where stuff will be installed to
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}" CACHE PATH "The subdirectory to the header prefix")

###############################################################################
# Debug and Testing (non permanent subdirs)
###############################################################################

IF(ISIXE_WITH_TESTING)
	enable_testing()
ENDIF(ISIXE_WITH_TESTING)

include_directories(${CMAKE_BINARY_DIR}/include/)

set(I6ENGINE_UTILS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-utils/include)
set(I6ENGINE_MATH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-math/include)
set(I6ENGINE_CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-core/include)
set(I6ENGINE_MODULES_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-modules/include)
set(I6ENGINE_RPG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-rpg/include)
set(I6ENGINE_EDITOR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tools/i6engine-editor/include)
set(I6ENGINE_LUABIND_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/deps/luabind/include)
set(I6ENGINE_PARTICLEUNIVERSE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/plugins/particleUniverse/include)

#####################################################################################
# deps we always build ourself
#####################################################################################

add_subdirectory(deps)

#####################################################################################
# libs
#####################################################################################

add_subdirectory(libs)

#####################################################################################
# scripting exports
#####################################################################################

IF(ISIXE_SCRIPTING)
	add_subdirectory(scripting)
ENDIF()

#####################################################################################
# test
#####################################################################################

IF(ISIXE_WITH_TESTING)
	add_subdirectory(test)
ENDIF(ISIXE_WITH_TESTING)

#####################################################################################
# plugins
#####################################################################################

add_subdirectory(plugins)

#####################################################################################
# samples
#####################################################################################

IF(ISIXE_WITH_SAMPLES)
	add_subdirectory(samples)
ENDIF(ISIXE_WITH_SAMPLES)

#####################################################################################
# tools
#####################################################################################

IF(ISIXE_WITH_TOOLS)
	add_subdirectory(tools)
ENDIF(ISIXE_WITH_TOOLS)

###############################################################################
# Docs
###############################################################################

IF(ISIXE_WITH_DOCU)
	IF(DOXYGEN_FOUND)
		add_subdirectory(docs)
	ENDIF(DOXYGEN_FOUND)
ENDIF(ISIXE_WITH_DOCU)

###############################################################################
# Tutorials
###############################################################################

IF(ISIXE_WITH_TUTORIALS)
	find_package(LATEX)
	add_subdirectory(tutorials)
ENDIF(ISIXE_WITH_TUTORIALS)

###########
# Final Statements
##########

install(FILES ${CMAKE_SOURCE_DIR}/config/ogre.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/config)
install(FILES ${CMAKE_SOURCE_DIR}/config/resources.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/config)
install(FILES ${CMAKE_SOURCE_DIR}/config/plugins.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/config)
install(FILES ${CMAKE_SOURCE_DIR}/config/i6engine.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/config)

IF(ISIXE_WITH_EDITOR)
	install(FILES ${CMAKE_BINARY_DIR}/bin/de_DE.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
	install(FILES ${CMAKE_BINARY_DIR}/bin/en_US.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
ENDIF(ISIXE_WITH_EDITOR)

IF(ISIXE_WITH_RPG)
	install(FILES ${CMAKE_SOURCE_DIR}/config/RPG.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/config)
ENDIF(ISIXE_WITH_RPG)

configure_file(${CMAKE_SOURCE_DIR}/config/resources.cfg ${CMAKE_BINARY_DIR}/bin/resources.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/ogre.cfg ${CMAKE_BINARY_DIR}/bin/ogre.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/plugins.cfg ${CMAKE_BINARY_DIR}/bin/plugins.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/i6engine.ini ${CMAKE_BINARY_DIR}/bin/i6engine.ini COPYONLY)

IF(ISIXE_WITH_RPG)
	configure_file(${CMAKE_SOURCE_DIR}/config/RPG.ini ${CMAKE_BINARY_DIR}/bin/RPG.ini COPYONLY)
ENDIF(ISIXE_WITH_RPG)

IF(ISIXE_WITH_PROFILING)
	configure_file(${CMAKE_SOURCE_DIR}/config/profiling.info ${CMAKE_BINARY_DIR}/bin/profiling.info COPYONLY)
ENDIF(ISIXE_WITH_PROFILING)

include(cmake/addToBinDir.cmake)

IF(UNIX)
	configure_file(${CMAKE_SOURCE_DIR}/config/test-buildconfigurations.sh.in ${CMAKE_BINARY_DIR}/test-buildconfigurations.sh @ONLY)
	add_custom_target(TestConfigurations COMMAND ./test-buildconfigurations.sh WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF(UNIX)

IF(WIN32)	
	configure_file(${CMAKE_SOURCE_DIR}/config/build-archive.bat.in ${CMAKE_BINARY_DIR}/build-archive.bat @ONLY)
	configure_file(${CMAKE_SOURCE_DIR}/config/build-prebuilt-msvc.bat.in ${CMAKE_BINARY_DIR}/build-prebuilt-msvc.bat @ONLY)
	configure_file(${CMAKE_SOURCE_DIR}/config/test-buildconfigurations.bat.in ${CMAKE_BINARY_DIR}/test-buildconfigurations.bat @ONLY)

	IF(ISIXE_WITH_TOOLS)	
		install(FILES ${CMAKE_SOURCE_DIR}/config/qt.conf DESTINATION ${CMAKE_INSTALL_PREFIX}/config)
	ENDIF(ISIXE_WITH_TOOLS)

	add_custom_target(BuildReleaseSourceArchive COMMAND build-archive.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(BuildRevisionSourceArchive COMMAND build-archive.bat packageRevision WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(TestConfigurations COMMAND test-buildconfigurations.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(BuildReleasePackages COMMAND build-prebuilt-msvc.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF(WIN32)

# set symlinks if it is an out-of-source-build
IF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	FILE(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/media/*)
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/media ${CMAKE_BINARY_DIR}/media)
ENDIF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

install(DIRECTORY media DESTINATION ${CMAKE_INSTALL_PREFIX})
