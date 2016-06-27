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
# SampleRPG cmake
################################

include_directories(include)
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
include_directories(${I6ENGINE_RPG_INCLUDE_DIR})

include_directories(${I6ENGINE_LUA_INCLUDE_DIR})
include_directories(${I6ENGINE_LUABIND_INCLUDE_DIR})

set(SampleRPGSrc
	main.cpp

	src/RPGApplication.cpp
)

if(WIN32)
   set(WINDOWS_RC_FILES
         ${CMAKE_SOURCE_DIR}/config/icon.rc
         )
    source_group("Resources" FILES ${WINDOWS_RC_FILES})
endif()

add_executable(SampleRPG ${GUI_TYPE} ${SampleRPGSrc} ${WINDOWS_RC_FILES})
set_target_properties(SampleRPG PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SampleRPG i6engine-rpg)

IF(UNIX)
	target_link_libraries(SampleRPG pthread)
ENDIF(UNIX)

IF(WIN32)
	add_custom_command(TARGET SampleRPG POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/SampleRPG.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS SampleRPG
		DESTINATION ${CMAKE_INSTALL_PREFIX}/samples
	)
