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
# editorRPGInitialization plugin cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
include_directories(${I6ENGINE_RPG_INCLUDE_DIR})
include_directories(${I6ENGINE_EDITOR_INCLUDE_DIR})

set(srcdir ./src)
SET(includedir ./include/)

set(pluginEditorRPGInitializationSrc
	${srcdir}/EditorRPGInitialization.cpp
)

QT5_WRAP_CPP(pluginEditorRPGInitializationInclude
	${includedir}/EditorRPGInitialization.h
)

ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

add_library(Plugin_EditorRPGInitialization SHARED ${pluginEditorRPGInitializationSrc} ${pluginEditorRPGInitializationInclude})

target_link_libraries(Plugin_EditorRPGInitialization i6engine-rpg ${QT_LIBRARIES})

add_custom_command(TARGET Plugin_EditorRPGInitialization PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/plugins/editor/initialization)

IF(WIN32)
	add_custom_command(TARGET Plugin_EditorRPGInitialization POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/Plugin_EditorRPGInitialization.dll ${CMAKE_BINARY_DIR}/bin/plugins/editor/initialization)
ELSEIF(UNIX)
	set_target_properties(Plugin_EditorRPGInitialization PROPERTIES PREFIX "")
	add_custom_command(TARGET Plugin_EditorRPGInitialization POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink Plugin_EditorRPGInitialization.so ${CMAKE_BINARY_DIR}/bin/plugins/editor/initialization/Plugin_EditorRPGInitialization.so)
ENDIF(WIN32)

install(TARGETS Plugin_EditorRPGInitialization
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/plugins/editor/initialization
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
