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
# takeControlRPG plugin cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_LUABIND_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
include_directories(${I6ENGINE_RPG_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/tools/TakeControl/include)

set(srcdir ./src)
SET(includedir ./include/)

set(plugintakeControlRPGSrc
	${srcdir}/TakeControlRPG.cpp
)

QT5_WRAP_CPP(pluginTakeControlRPGInclude
	${includedir}/TakeControlRPG.h
)

ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

add_library(Plugin_TakeControlRPG SHARED ${plugintakeControlRPGSrc} ${pluginTakeControlRPGInclude})

target_link_libraries(Plugin_TakeControlRPG i6engine-rpg ${QT_LIBRARIES})

add_custom_command(TARGET Plugin_TakeControlRPG PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/plugins/takeControl/dialog)

IF(WIN32)
	add_custom_command(TARGET Plugin_TakeControlRPG POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/Plugin_TakeControlRPG.dll ${CMAKE_BINARY_DIR}/bin/plugins/takeControl/dialog)
ELSEIF(UNIX)
	set_target_properties(Plugin_TakeControlRPG PROPERTIES PREFIX "")
	add_custom_command(TARGET Plugin_TakeControlRPG POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink Plugin_TakeControlRPG.so ${CMAKE_BINARY_DIR}/bin/plugins/takeControl/dialog/Plugin_TakeControlRPG.so)
ENDIF(WIN32)

install(TARGETS Plugin_TakeControlRPG
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/plugins/takeControl/dialog
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
