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

########################
# lua install
########################

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS lua
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
