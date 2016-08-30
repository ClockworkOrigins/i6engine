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

cmake_minimum_required (VERSION 2.8.12)

project(lua)

IF(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

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
	set (CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} -g")
	set (CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} -0s -DNDEBUG")
	set (CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} -O3 -DNDEBUG")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set (CXX_FLAGS "/MP /W4 /wd4127 /DNOMINMAX")
	set (CMAKE_CXX_FLAGS_DEBUG		"${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FLAGS}")
	set (CMAKE_CXX_FLAGS_MINSIZEREL	"${CMAKE_CXX_FLAGS_MINSIZEREL} ${CXX_FLAGS}")
	set (CMAKE_CXX_FLAGS_RELEASE		"${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS}")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CXX_FLAGS}")
ELSE()
	message (SEND_INFO "Unknown C++ compiler \"${CMAKE_CXX_COMPILER_ID}\". Only GNU, Clang and MSVC are completely supported.")
ENDIF()

################################
# lua cmake
################################

include_directories("src")

set(srcdir ./src)

set(luaSrc
	${srcdir}/lapi.c
	${srcdir}/lauxlib.c
	${srcdir}/lbaselib.c
	${srcdir}/lbitlib.c
	${srcdir}/lcode.c
	${srcdir}/lcorolib.c
	${srcdir}/lctype.c
	${srcdir}/ldblib.c
	${srcdir}/ldebug.c
	${srcdir}/ldo.c
	${srcdir}/ldump.c
	${srcdir}/lfunc.c
	${srcdir}/lgc.c
	${srcdir}/linit.c
	${srcdir}/liolib.c
	${srcdir}/llex.c
	${srcdir}/lmathlib.c
	${srcdir}/lmem.c
	${srcdir}/loadlib.c
	${srcdir}/lobject.c
	${srcdir}/lopcodes.c
	${srcdir}/loslib.c
	${srcdir}/lparser.c
	${srcdir}/lstate.c
	${srcdir}/lstring.c
	${srcdir}/lstrlib.c
	${srcdir}/ltable.c
	${srcdir}/ltablib.c
	${srcdir}/ltm.c
	${srcdir}/lundump.c
	${srcdir}/lutf8lib.c
	${srcdir}/lvm.c
	${srcdir}/lzio.c
)

add_library(lua SHARED ${luaSrc})

IF(UNIX)
	target_compile_options(lua PRIVATE "-DLUA_USE_LINUX -DLUA_DL_DLOPEN")
	target_link_libraries(lua dl)
ENDIF(UNIX)
set_target_properties(lua PROPERTIES POSITION_INDEPENDENT_CODE ON)

########################
# lua install
########################

install(FILES src/lua.h src/lauxlib.h src/luaconf.h src/lapi.h src/lcode.h src/lctype.h src/ldebug.h src/ldo.h src/lfunc.h src/lgc.h src/llex.h src/llimits.h src/lmem.h src/lobject.h src/lopcodes.h src/lparser.h src/lprefix.h src/lstate.h src/lstring.h src/ltable.h src/ltm.h src/lualib.h src/lundump.h src/lvm.h src/lzio.h  DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

install(TARGETS lua
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
