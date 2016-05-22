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

################################
# lua cmake
################################

include_directories("include")
include_directories("include/i6engine")
include_directories("include/i6engine/lua")

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

add_library(lua STATIC ${luaSrc})

IF(UNIX)
	target_compile_options(lua PRIVATE "-DLUA_USE_LINUX")
	target_link_libraries(lua dl)
ENDIF(UNIX)
set_target_properties(lua PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(luap ${srcdir}/lua.c)
target_link_libraries(luap lua)

IF(UNIX)
	target_link_libraries(luap m)
ENDIF(UNIX)

IF(WIN32)
	add_custom_command(TARGET luap POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/luap.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

########################
# lua install
########################

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS lua
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
