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
OPTION(ISIXE_WITH_CONSOLE "build samples with console output (off affects only Windows build)" ON)

IF(NOT ISIXE_WITH_CONSOLE AND WIN32)
	SET(GUI_TYPE WIN32)
ENDIF(NOT ISIXE_WITH_CONSOLE AND WIN32)

# options are
# * ""
# * "python"
# * "lua"
set(ISIXE_WITH_SCRIPTING "python" CACHE STRING "specifies the scripting system to use" )

IF(WIN32)
	IF(CMAKE_GENERATOR MATCHES "Visual Studio 12")
		SET(DEP_DIR_BUILD "msvc12_")
	ELSEIF(CMAKE_GENERATOR MATCHES "Visual Studio 14")
		SET(DEP_DIR_BUILD "msvc14_")
	ELSE()
		MESSAGE([WARNING] "Toolchain not supported yet!")
	ENDIF()

	IF(CMAKE_GENERATOR MATCHES "Win64")
		SET(DEP_DIR_BUILD "${DEP_DIR_BUILD}x64")
	ELSEIF(CMAKE_GENERATOR MATCHES "ARM")
		SET(DEP_DIR_BUILD "${DEP_DIR_BUILD}arm")
	ELSE()
		SET(DEP_DIR_BUILD "${DEP_DIR_BUILD}x86")
	ENDIF()
ENDIF()

SET(ISIXE_DEP_DIR "${CMAKE_SOURCE_DIR}/dependencies/${DEP_DIR_BUILD}" CACHE STRING "path to dependencies for i6engine")

IF(ISIXE_WITH_TESTING AND NOT WIN32)
	# static for gmock
	option(ISIXE_BUILD_SHARED "build project with shared libraries" OFF)
ELSE(ISIXE_WITH_TESTING AND NOT WIN32)
	option(ISIXE_BUILD_SHARED "build project with shared libraries" ON)
ENDIF(ISIXE_WITH_TESTING AND NOT WIN32)

project(i6engine)

IF(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(ISIXE_WITH_SCRIPTING STREQUAL "lua")
	set(SCRIPTING_LUA ON)
	set(SCRIPTING_PYTHON OFF)
	set(ISIXE_SCRIPTING "SCRIPTING_LUA")
ELSEIF(ISIXE_WITH_SCRIPTING STREQUAL "python")
	set(SCRIPTING_PYTHON ON)
	set(SCRIPTING_LUA OFF)
	set(ISIXE_SCRIPTING "SCRIPTING_PYTHON")
ELSE()
	set(SCRIPTING_LUA OFF)
	set(SCRIPTING_PYTHON OFF)
	set(ISIXE_SCRIPTING "SCRIPTING_NONE")
ENDIF()

# Setting last version
SET(LAST_VERSION_MAJOR 0)
SET(LAST_VERSION_MINOR 9)
SET(LAST_VERSION_PATCH 0)

# Setting current version
SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 0)
SET(VERSION_PATCH 0)

#########################################################################
# Global Macros and Definitions
#########################################################################

# Sets appropriate Compilerflags

set (CXX_FLAGS "-pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wredundant-decls -Wctor-dtor-privacy -Wsign-conversion -Wno-unused-parameter -Wno-long-long -std=c++11")
IF(ISIXE_WITH_TESTING)
	set(CXX_FLAGS "${CXX_FLAGS} -fPIC")
ENDIF(ISIXE_WITH_TESTING)
set (CLANG_FLAGS "-Wstring-plus-int")

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	set (CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} ${CLANG_FLAGS} -g")
	set (CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} ${CLANG_FLAGS} -0s -DNDEBUG")
	set (CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} ${CLANG_FLAGS} -O3 -DNDEBUG")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} ${CLANG_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	IF(NOT ${CMAKE_CXX_COMPILER_VERSION} LESS 4.9)
		set(CXX_FLAGS "${CXX_FLAGS} -fdiagnostics-color=always -fsanitize=leak")
	ENDIF()
	set (CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} -g")
	set (CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} -0s -DNDEBUG")
	set (CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} -O3 -DNDEBUG")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set (CMAKE_CXX_FLAGS_DEBUG		"${CMAKE_CXX_FLAGS_DEBUG} /MP")
	set (CMAKE_CXX_FLAGS_MINSIZEREL	"${CMAKE_CXX_FLAGS_MINSIZEREL} /MP")
	set (CMAKE_CXX_FLAGS_RELEASE		"${CMAKE_CXX_FLAGS_RELEASE} /MP")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP /Qvec-report:1 /Qpar /Qpar-report:1")
ELSE()
	message (SEND_INFO "Unknown C++ compiler \"${CMAKE_CXX_COMPILER_ID}\". Only GNU, Clang and MSVC are completely supported.")
ENDIF()

#########################################################################
# Global Directory Definitions
#########################################################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/" ${CMAKE_MODULE_PATH})

#Setting Output Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

#########################################################################
# Library Section
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
	IF(ISIXE_WITH_SCRIPTING STREQUAL "python")
		SET(BOOST_COMPONENTS ${BOOST_COMPONENTS} python)
	ENDIF()
	IF(ISIXE_WITH_LOGGING)
		SET(BOOST_COMPONENTS ${BOOST_COMPONENTS} log log_setup)
	ENDIF(ISIXE_WITH_LOGGING)
	FOREACH(comp ${BOOST_COMPONENTS})
		list(APPEND libs boost_${comp})
	ENDFOREACH(comp)
	SET(BOOST_COMPONENTS ${libs})
	find_package(EasyFind REQUIRED COMPONENTS ${BOOST_COMPONENTS})
	include_directories(SYSTEM ${BOOST_INCLUDE_DIR})

#----------------------------------------------------
# Python
#----------------------------------------------------
IF(ISIXE_WITH_SCRIPTING STREQUAL "python")
	find_package(PythonLibs 2.7 REQUIRED)
	include_directories(${PYTHON_INCLUDE_DIRS})
ENDIF()

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
	SET(OIS_ROOT ${ISIXE_DEP_DIR}/misc)
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
IF(ISIXE_WITH_TESTING)
	SET(LIBNAME "GTEST")
	SET(LIBHEADER "gtest/gtest.h")
	SET(GTEST_ROOT ${ISIXE_DEP_DIR}/gmock)
	find_package(EasyFind REQUIRED COMPONENTS gmock_main gmock gtest gtest_main)
	include_directories(SYSTEM ${GTEST_INCLUDE_DIR})
ENDIF(ISIXE_WITH_TESTING)

#----------------------------------------------------
# Google Performance Tools
#----------------------------------------------------
IF(ISIXE_WITH_PROFILING)
	SET(LIBNAME "GPerfTools")
	SET(LIBHEADER "gperftools/tcmalloc.h")
	SET(GPerfTools_ROOT ${ISIXE_DEP_DIR}/gperftools)
	IF(WIN32)
		find_package(EasyFind REQUIRED COMPONENTS libtcmalloc_minimal)
	ELSE(WIN32)
		find_package(EasyFind REQUIRED COMPONENTS tcmalloc profiler)
	ENDIF(WIN32)
	include_directories(SYSTEM ${GPerfTools_INCLUDE_DIR})
ENDIF(ISIXE_WITH_PROFILING)

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
	find_package(EasyFind REQUIRED COMPONENTS clock_compression clock_iniParser clock_sockets)
	include_directories(SYSTEM ${CLOCKUTILS_INCLUDE_DIR})

#----------------------------------------------------
# JPEG
#----------------------------------------------------
IF(NOT WIN32)
	find_package(JPEG REQUIRED)
ENDIF(NOT WIN32)

#----------------------------------------------------
# wxWidgets
#----------------------------------------------------
IF(ISIXE_WITH_TOOLS)
	SET(LIBNAME "WXWIDGETS")
	SET(WXWIDGETS_ROOT ${ISIXE_DEP_DIR}/wxWidgets)
	IF(WIN32)
		SET(LIBHEADER "wx/wx/wx.h")
		find_package(EasyFind REQUIRED COMPONENTS wxbase30u wxbase30u_net wxbase30u_xml wxexpat wxjpeg wxmsw30u_adv wxmsw30u_aui wxmsw30u_core wxmsw30u_gl wxmsw30u_html wxmsw30u_media wxmsw30u_propgrid wxmsw30u_qa wxmsw30u_ribbon wxmsw30u_richtext wxmsw30u_stc wxmsw30u_webview wxmsw30u_xrc wxpng wxregexu wxscintilla wxtiff wxzlib)
		include_directories(SYSTEM ${WXWIDGETS_INCLUDE_DIR}/wx)
	ELSEIF(UNIX)
		SET(LIBHEADER "wx/wx.h")
		find_package(EasyFind REQUIRED COMPONENTS wx_baseu-3.0 wx_baseu_net-3.0 wx_baseu_xml-3.0 wx_gtk2u_adv-3.0 wx_gtk2u_aui-3.0 wx_gtk2u_core-3.0 wx_gtk2u_html-3.0 wx_gtk2u_qa-3.0 wx_gtk2u_propgrid-3.0 wx_gtk2u_richtext-3.0 wx_gtk2u_xrc-3.0)
	ENDIF(WIN32)
	include_directories(SYSTEM ${WXWIDGETS_INCLUDE_DIR})
ENDIF(ISIXE_WITH_TOOLS)

#----------------------------------------------------
# GTK
#----------------------------------------------------
IF(ISIXE_WITH_TOOLS AND UNIX)
	find_package(GTK2 REQUIRED gtk)
	INCLUDE_DIRECTORIES(SYSTEM ${GTK2_INCLUDE_DIRS})
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GLIB2 glib-2.0)
	INCLUDE_DIRECTORIES(SYSTEM ${GLIB2_INCLUDE_DIRS})
ENDIF(ISIXE_WITH_TOOLS AND UNIX)

###############################################################################
# Configure Build Settings (has to be before building m2etis because m2etis needs i6engineBuildSettings)
###############################################################################

configure_file(${CMAKE_SOURCE_DIR}/config/i6engineBuildSettings.h.in ${CMAKE_BINARY_DIR}/include/i6engine/i6engineBuildSettings.h @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/include/i6engine/i6engineBuildSettings.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/i6engine)

#----------------------------------------------------
# m2etis
#----------------------------------------------------
IF(ISIXE_WITH_NETWORK)
	# Build m2etis
	IF(WIN32)
		IF(CMAKE_GENERATOR MATCHES "Visual Studio 12")
			SET(METIS_TOOLCHAIN msvc12)
		ELSE()
			MESSAGE([ERROR] "Generator not supported yet!")
		ENDIF()
		IF(CMAKE_GENERATOR MATCHES "Win64")
			SET(METIS_ARCH 64)
		ELSE(CMAKE_GENERATOR MATCHES "Win64")
			SET(METIS_ARCH 32)
		ENDIF(CMAKE_GENERATOR MATCHES "Win64")
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-m2etis.bat ${METIS_TOOLCHAIN} ${METIS_ARCH} "${ISIXE_DEP_DIR}/.." "${CMAKE_BINARY_DIR}/include" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ELSE(WIN32)
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-m2etis.sh "${CMAKE_BINARY_DIR}/include/i6engine" "${CMAKE_BUILD_TYPE}" "${ISIXE_DEP_DIR}" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(WIN32)

	#----------------------------------------------------
	# I6M2ETIS
	#----------------------------------------------------
	SET(LIBNAME "M2etis")
	SET(LIBHEADER "m2etis/pubsub/PubSubSystem.h")
	SET(M2etis_ROOT ${ISIXE_DEP_DIR}/m2etis)
	find_package(EasyFind REQUIRED COMPONENTS m2etis)
	include_directories(SYSTEM ${M2etis_INCLUDE_DIR})
ENDIF(ISIXE_WITH_NETWORK)

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
set(I6ENGINE_LUA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/deps/lua/include)
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
install(FILES ${CMAKE_SOURCE_DIR}/config/pued.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/config)
install(FILES ${CMAKE_SOURCE_DIR}/config/i6engine.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/config)

IF(ISIXE_WITH_RPG)
	install(FILES ${CMAKE_SOURCE_DIR}/config/RPG.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/config)
ENDIF(ISIXE_WITH_RPG)

configure_file(${CMAKE_SOURCE_DIR}/config/resources.cfg ${CMAKE_BINARY_DIR}/bin/resources.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/ogre.cfg ${CMAKE_BINARY_DIR}/bin/ogre.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/plugins.cfg ${CMAKE_BINARY_DIR}/bin/plugins.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/pued.cfg ${CMAKE_BINARY_DIR}/bin/pued.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/config/i6engine.ini ${CMAKE_BINARY_DIR}/bin/i6engine.ini COPYONLY)

IF(ISIXE_WITH_RPG)
	configure_file(${CMAKE_SOURCE_DIR}/config/RPG.ini ${CMAKE_BINARY_DIR}/bin/RPG.ini COPYONLY)
ENDIF(ISIXE_WITH_RPG)

IF(ISIXE_WITH_PROFILING)
	configure_file(${CMAKE_SOURCE_DIR}/config/profiling.info ${CMAKE_BINARY_DIR}/bin/profiling.info COPYONLY)
ENDIF(ISIXE_WITH_PROFILING)

IF(UNIX)
	file(GLOB files "${ISIXE_DEP_DIR}/ogre/lib/OGRE/*.so.*")
	foreach(filename ${files})
		get_filename_component(filename_pure ${filename} NAME)
		execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filename} ${CMAKE_BINARY_DIR}/bin/${filename_pure})
	endforeach()

	configure_file(${CMAKE_SOURCE_DIR}/config/test-buildconfigurations.sh.in ${CMAKE_BINARY_DIR}/test-buildconfigurations.sh @ONLY)
	add_custom_target(TestConfigurations COMMAND ./test-buildconfigurations.sh WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF(UNIX)

IF(WIN32)
	file(GLOB_RECURSE files "${ISIXE_DEP_DIR}/*.dll")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${filename} ${CMAKE_BINARY_DIR}/bin)
	endforeach()
	
	configure_file(${CMAKE_SOURCE_DIR}/config/build-archive.bat.in ${CMAKE_BINARY_DIR}/build-archive.bat @ONLY)
	configure_file(${CMAKE_SOURCE_DIR}/config/test-buildconfigurations.bat.in ${CMAKE_BINARY_DIR}/test-buildconfigurations.bat @ONLY)

	add_custom_target(BuildReleaseSourceArchive COMMAND build-archive.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(BuildRevisionSourceArchive COMMAND build-archive.bat packageRevision WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(TestConfigurations COMMAND test-buildconfigurations.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF(WIN32)

# set symlinks if it is an out-of-source-build
IF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/media ${CMAKE_BINARY_DIR}/media)
ENDIF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

install(DIRECTORY media DESTINATION ${CMAKE_INSTALL_PREFIX})
