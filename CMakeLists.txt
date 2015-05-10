##
# Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##

# CMake main configfile i6engine project
#

cmake_minimum_required (VERSION 2.8)

#########################################################################
#########################################################################
#
# Global Build Configuration Options
#
#########################################################################

option(ISIXE_LOGGING "enables Logging" OFF)
option(ISIXE_NETWORK "build i6engine with networking capabilities" OFF)
option(ISIXE_PROFILING "build i6engine with profiling" OFF)
option(ISIXE_WITH_LUA_SCRIPTING "build lua scripting module" ON)
option(ISIXE_WITH_PYTHON_SCRIPTING "build python scripting module" ON)
option(ISIXE_TESTING "build i6engine with tests" OFF)
OPTION(ISIXE_WITH_AUDIO "build i6engine with OpenAL Soft audio subsystem" ON)
OPTION(ISIXE_WITH_EDITOR "build i6engine with the level editor" ON)
OPTION(ISIXE_WITH_SAMPLES "build some samples for i6engine" ON)
OPTION(ISIXE_WITH_TOOLS "build some tools for i6engine" ON)
OPTION(ISIXE_WITH_RPG "build rpg module for i6engine" ON)
OPTION(ISIXE_WITH_DOCU "build documentation for i6engine" ON)
OPTION(ISIXE_WITH_TUTORIALS "build tutorials for i6engine" ON)
set(ISIXE_DEP_DIR "${CMAKE_SOURCE_DIR}/dependencies" CACHE STRING "path to dependencies for i6engine")

IF(ISIXE_TESTING AND NOT WIN32)
	# static for gmock
	option(ISIXE_BUILD_SHARED "build project with shared libraries" OFF)
ELSE(ISIXE_TESTING AND NOT WIN32)
	option(ISIXE_BUILD_SHARED "build project with shared libraries" ON)
ENDIF(ISIXE_TESTING AND NOT WIN32)

project(i6engine)

IF(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(ISIXE_WITH_RPG AND NOT ISIXE_WITH_LUA_SCRIPTING)
	MESSAGE(SEND_ERROR "rpg module requires lua scripting enabled to use all features!")
ENDIF(ISIXE_WITH_RPG AND NOT ISIXE_WITH_LUA_SCRIPTING)

#########################################################################
# Global Macros and Definitions
#########################################################################

# Sets appropriate Compilerflags

set (CXX_FLAGS "-pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wredundant-decls -Wctor-dtor-privacy -Wsign-conversion -Wno-unused-parameter -Wno-long-long -std=c++11")
IF(ISIXE_TESTING)
	set(CXX_FLAGS "${CXX_FLAGS} -fPIC")
ENDIF(ISIXE_TESTING)
set (CLANG_FLAGS "-Wstring-plus-int")

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	set (CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} ${CLANG_FLAGS} -g")
	set (CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} ${CLANG_FLAGS} -0s -DNDEBUG")
	set (CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} ${CLANG_FLAGS} -O3 -DNDEBUG -Werror")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} ${CLANG_FLAGS} -O2 -g -Werror")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	IF(NOT ${CMAKE_CXX_COMPILER_VERSION} LESS 4.9)
		set(CXX_FLAGS "${CXX_FLAGS} -fdiagnostics-color=always -fsanitize=leak")
	ENDIF()
	set (CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} -g")
	set (CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} -0s -DNDEBUG")
	set (CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} -O3 -DNDEBUG -Werror")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} -O2 -g -Werror")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set (CMAKE_CXX_FLAGS_DEBUG		"/Z7 /Od")
	set (CMAKE_CXX_FLAGS_MINSIZEREL		"/O1 /Os")
	set (CMAKE_CXX_FLAGS_RELEASE		"/Ox /MP /DNDEBUG /Ob2 /Oi /Ot /Oy")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"/Ox /MP /Z7 /Ob2 /Oi /Ot /Oy")
ELSE()
	message (SEND_INFO "Unknown C++ compiler \"${CMAKE_CXX_COMPILER_ID}\". Only GNU and Clang are completely supported.")
ENDIF()

#########################################################################
#Global Directory Definitions
#########################################################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/" ${CMAKE_MODULE_PATH})

#Setting Output Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

IF(APPLE)
	set(CMAKE_INSTALL_NAME_DIR @executable_path)
	set(CMAKE_BUILD_ISIXE_INSTALL_RPATH ON)
ENDIF(APPLE)

#########################################################################
#Library Section
#########################################################################

#----------------------------------------------------
# Pthreads
#----------------------------------------------------
IF(NOT WIN32)
	find_package(Pthreads REQUIRED)
	include_directories(SYSTEM ${PTHREADS_INCLUDE_DIR})
ENDIF(NOT WIN32)

#----------------------------------------------------
# Boost
#---------------------------------------------------
	SET(LIBNAME "BOOST")
	SET(LIBHEADER "boost/thread.hpp")
	SET(BOOST_ROOT ${ISIXE_DEP_DIR}/boost)
	set(BOOST_COMPONENTS chrono date_time filesystem serialization system thread)
	IF(ISIXE_WITH_PYTHON_SCRIPTING)
		SET(BOOST_COMPONENTS ${BOOST_COMPONENTS} python)
	ENDIF(ISIXE_WITH_PYTHON_SCRIPTING)
	IF(ISIXE_LOGGING)
		SET(BOOST_COMPONENTS ${BOOST_COMPONENTS} log log_setup)
	ENDIF(ISIXE_LOGGING)
	FOREACH(comp ${BOOST_COMPONENTS})
		list(APPEND libs boost_${comp})
	ENDFOREACH(comp)
	SET(BOOST_COMPONENTS ${libs})
	find_package(EasyFind REQUIRED COMPONENTS ${BOOST_COMPONENTS})
	include_directories(SYSTEM ${BOOST_INCLUDE_DIR})
	
#----------------------------------------------------
# Python
#----------------------------------------------------
IF(ISIXE_WITH_PYTHON_SCRIPTING)
	find_package(PythonLibs 2.7 REQUIRED)
	include_directories(${PYTHON_INCLUDE_DIRS})
ENDIF(ISIXE_WITH_PYTHON_SCRIPTING)

#----------------------------------------------------
# Ogre3D
#----------------------------------------------------
	SET(LIBNAME "OGRE")
	SET(LIBHEADER "OGRE/Ogre.h")
	SET(OGRE_ROOT ${ISIXE_DEP_DIR}/ogre)
	find_package(EasyFind REQUIRED COMPONENTS OgreMain OgreOverlay OgrePaging OgreProperty OgreRTShaderSystem OgreTerrain)
	include_directories(SYSTEM ${OGRE_INCLUDE_DIR})
	include_directories(SYSTEM ${OGRE_INCLUDE_DIR}/OGRE/)
	include_directories(SYSTEM ${OGRE_INCLUDE_DIR}/OGRE/Paging)
	include_directories(SYSTEM ${OGRE_INCLUDE_DIR}/OGRE/Terrain)

#----------------------------------------------------
# OIS
#----------------------------------------------------
	SET(LIBNAME "OIS")
	SET(LIBHEADER "OIS/OIS.h")
	SET(OIS_ROOT ${ISIXE_DEP_DIR}/ois)
	find_package(EasyFind REQUIRED COMPONENTS OIS)
	include_directories(SYSTEM ${OIS_INCLUDE_DIR})

#----------------------------------------------------
# OpenAL Soft
#----------------------------------------------------
IF(ISIXE_WITH_AUDIO)
	SET(LIBNAME "OPENALSOFT")
	SET(LIBHEADER "AL/al.h")
	SET(OPENALSOFT_ROOT ${ISIXE_DEP_DIR}/OpenAL)
	IF(WIN32)
		find_package(EasyFind REQUIRED COMPONENTS OpenAL32)
	ELSEIF(UNIX)
		find_package(EasyFind REQUIRED COMPONENTS openal)
	ENDIF()
	include_directories(SYSTEM ${OPENALSOFT_INCLUDE_DIR})
ENDIF(ISIXE_WITH_AUDIO)

#----------------------------------------------------
# Bullet
#----------------------------------------------------
	SET(LIBNAME "BULLET")
	SET(LIBHEADER "bullet/btBulletCollisionCommon.h")
	SET(BULLET_ROOT ${ISIXE_DEP_DIR}/bullet)
	find_package(EasyFind REQUIRED COMPONENTS BulletCollision BulletDynamics LinearMath BulletWorldImporter BulletFileLoader)
	include_directories(SYSTEM ${BULLET_INCLUDE_DIR})
	include_directories(SYSTEM ${BULLET_INCLUDE_DIR}/bullet)

#----------------------------------------------------
# CEGUI
#----------------------------------------------------
	SET(LIBNAME "CEGUI")
	SET(LIBHEADER "CEGUI/CEGUI.h")
	SET(CEGUI_ROOT ${ISIXE_DEP_DIR}/cegui)
	find_package(EasyFind REQUIRED COMPONENTS CEGUIBase-0 CEGUIExpatParser CEGUIOgreRenderer-0)
	include_directories(SYSTEM ${CEGUI_INCLUDE_DIR})

#----------------------------------------------------
# Google Test
#----------------------------------------------------
IF(ISIXE_TESTING)
	SET(LIBNAME "GTEST")
	SET(LIBHEADER "gtest/gtest.h")
	SET(GTEST_ROOT ${ISIXE_DEP_DIR}/gmock)
	find_package(EasyFind REQUIRED COMPONENTS gmock_main gmock gtest gtest_main)
	include_directories(SYSTEM ${GTEST_INCLUDE_DIR})
ENDIF(ISIXE_TESTING)

#----------------------------------------------------
# Google Performance Tools
#----------------------------------------------------
IF(ISIXE_PROFILING)
	SET(LIBNAME "GPerfTools")
	SET(LIBHEADER "gperftools/tcmalloc.h")
	SET(GPerfTools_ROOT ${ISIXE_DEP_DIR}/gperftools)
	IF(WIN32)
		find_package(EasyFind REQUIRED COMPONENTS libtcmalloc_minimal)
	ELSE(WIN32)
		find_package(EasyFind REQUIRED COMPONENTS tcmalloc profiler)
	ENDIF(WIN32)
	include_directories(SYSTEM ${GPerfTools_INCLUDE_DIR})
ENDIF(ISIXE_PROFILING)

#----------------------------------------------------
# TinyXML
#----------------------------------------------------
	SET(LIBNAME "TinyXML")
	SET(LIBHEADER "tinyxml2.h")
	SET(TinyXML_ROOT ${ISIXE_DEP_DIR}/tinyxml2)
	find_package(EasyFind REQUIRED COMPONENTS tinyxml2)
	include_directories(SYSTEM ${TinyXML_INCLUDE_DIR})

#----------------------------------------------------
# clockUtils
#----------------------------------------------------
	SET(LIBNAME "CLOCKUTILS")
	SET(LIBHEADER "clockUtils/errors.h")
	SET(CLOCKUTILS_ROOT ${ISIXE_DEP_DIR}/clockUtils)
	find_package(EasyFind REQUIRED COMPONENTS clock_iniParser)
	include_directories(SYSTEM ${CLOCKUTILS_INCLUDE_DIR})

#----------------------------------------------------
# JPEG
#----------------------------------------------------
IF(NOT WIN32)
	find_package(JPEG REQUIRED)
ENDIF(NOT WIN32)

###############################################################################
# Configure Build Settings (has to be before building m2etis because m2etis needs i6engineBuildSettings)
###############################################################################

configure_file(${CMAKE_SOURCE_DIR}/config/i6engineBuildSettings.h.in ${CMAKE_BINARY_DIR}/include/i6engine/i6engineBuildSettings.h @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/include/i6engine/i6engineBuildSettings.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/i6engine)

#----------------------------------------------------
# m2etis
#----------------------------------------------------
IF(ISIXE_NETWORK)
	# Build m2etis
	IF(WIN32)
		execute_process(COMMAND ${ISIXE_DEP_DIR}/build-m2etis.bat "${ISIXE_DEP_DIR}" "${CMAKE_BINARY_DIR}/include")
	ELSE(WIN32)
		execute_process(COMMAND ${ISIXE_DEP_DIR}/build-m2etis.sh "${CMAKE_BINARY_DIR}/include/i6engine" "${CMAKE_BUILD_TYPE}")
	ENDIF(WIN32)

	#----------------------------------------------------
	# I6M2ETIS
	#----------------------------------------------------
	IF(ISIXE_NETWORK)
		SET(LIBNAME "M2etis")
		SET(LIBHEADER "m2etis/pubsub/PubSubSystem.h")
		SET(M2etis_ROOT ${ISIXE_DEP_DIR}/m2etis)
		find_package(EasyFind REQUIRED COMPONENTS m2etis)
		include_directories(SYSTEM ${M2etis_INCLUDE_DIR})
	ENDIF(ISIXE_NETWORK)
ENDIF(ISIXE_NETWORK)

####################################################################################
# Install paths
####################################################################################

set(LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" CACHE STRING "Library directory name")
## the following are directories where stuff will be installed to
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}" CACHE PATH "The subdirectory to the header prefix")

###############################################################################
# Debug and Testing (non permanent subdirs)
###############################################################################

IF(ISIXE_TESTING)
	enable_testing()
ENDIF(ISIXE_TESTING)

include_directories(${CMAKE_BINARY_DIR}/include/)

set(I6ENGINE_UTILS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-utils/include)
set(I6ENGINE_MATH_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-math/include)
set(I6ENGINE_CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-core/include)
set(I6ENGINE_MODULES_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-modules/include)
set(I6ENGINE_RPG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/i6engine-rpg/include)
set(I6ENGINE_EDITOR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tools/i6engine-editor/include)
set(I6ENGINE_LUA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/deps/lua/include)
set(I6ENGINE_LUABIND_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/deps/luabind/include)

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

IF(ISIXE_WITH_LUA_SCRIPTING OR ISIXE_WITH_PYTHON_SCRIPTING)
	add_subdirectory(scripting)
ENDIF(ISIXE_WITH_LUA_SCRIPTING OR ISIXE_WITH_PYTHON_SCRIPTING)

#####################################################################################
# test
#####################################################################################

IF(ISIXE_TESTING)
	add_subdirectory(test)
ENDIF(ISIXE_TESTING)

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
	find_package(Doxygen)
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

IF(ISIXE_PROFILING)
	configure_file(${CMAKE_SOURCE_DIR}/config/profiling.info ${CMAKE_BINARY_DIR}/bin/profiling.info COPYONLY)
ENDIF(ISIXE_PROFILING)

IF(UNIX)
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${ISIXE_DEP_DIR}/ogre/lib/OGRE ${CMAKE_BINARY_DIR}/bin/plugins)
ENDIF(UNIX)

IF(WIN32)
	file(GLOB_RECURSE files "${ISIXE_DEP_DIR}/*.dll")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${filename} ${CMAKE_BINARY_DIR}/bin)
	endforeach()

	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)
	execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/plugins)

	file(GLOB files "${CMAKE_BINARY_DIR}/bin/Plugin_*.dll")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${filename} ${CMAKE_BINARY_DIR}/bin/plugins)
	endforeach()

	execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${files})

	file(GLOB files "${CMAKE_BINARY_DIR}/bin/RenderSystem_*.dll")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${filename} ${CMAKE_BINARY_DIR}/bin/plugins)
	endforeach()

	execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${files})
ENDIF(WIN32)

# set symlinks if it is an out-of-source-build
IF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/media ${CMAKE_BINARY_DIR}/media)
ENDIF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

install(DIRECTORY media DESTINATION ${CMAKE_INSTALL_PREFIX})
