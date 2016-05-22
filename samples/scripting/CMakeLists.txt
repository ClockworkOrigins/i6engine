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
# SampleScripting cmake
################################

include_directories(include)
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

IF(SCRIPTING_LUA)
	include_directories(${I6ENGINE_LUA_INCLUDE_DIR})
	include_directories(${I6ENGINE_LUABIND_INCLUDE_DIR})
ENDIF(SCRIPTING_LUA)

set(SampleScriptingSrc
	main.cpp

	src/ScriptingApplication.cpp

	src/components/ScriptingShatterComponent.cpp
)

add_executable(SampleScripting ${GUI_TYPE} ${SampleScriptingSrc})
set_target_properties(SampleScripting PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SampleScripting SampleCommon)

IF(UNIX)
	target_link_libraries(SampleScripting pthread)
ENDIF(UNIX)

IF(WIN32)
	add_custom_command(TARGET SampleScripting POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/SampleScripting.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS SampleScripting
		DESTINATION ${CMAKE_INSTALL_PREFIX}/samples
	)
