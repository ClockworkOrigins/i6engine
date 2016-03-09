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
# editorSaveObjectRPG plugin cmake
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

set(pluginEditorSaveObjectRPGSrc
	${srcdir}/EditorSaveObjectRPG.cpp
)

QT5_WRAP_CPP(pluginEditorSaveObjectRPGInclude
	${includedir}/EditorSaveObjectRPG.h
)

IF(SCRIPTING_LUA)
	set(pluginEditorSaveObjectRPGSrc
		${pluginEditorSaveObjectRPGSrc}
		${srcdir}/EditorSaveObjectRPGLua.cpp
	)
ELSEIF(SCRIPTING_PYTHON)
	set(pluginEditorSaveObjectRPGSrc
		${pluginEditorSaveObjectRPGSrc}
		${srcdir}/EditorSaveObjectRPGPython.cpp
	)
ENDIF()

ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

add_library(Plugin_EditorSaveObjectRPG SHARED ${pluginEditorSaveObjectRPGSrc} ${pluginEditorSaveObjectRPGInclude})

target_link_libraries(Plugin_EditorSaveObjectRPG i6engine-rpg ${QT_LIBRARIES})

add_custom_command(TARGET Plugin_EditorSaveObjectRPG PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/plugins/editor/saveObject)

IF(WIN32)
	add_custom_command(TARGET Plugin_EditorSaveObjectRPG POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/Plugin_EditorSaveObjectRPG.dll ${CMAKE_BINARY_DIR}/bin/plugins/editor/saveObject)
ELSEIF(UNIX)
	set_target_properties(Plugin_EditorSaveObjectRPG PROPERTIES PREFIX "")
	add_custom_command(TARGET Plugin_EditorSaveObjectRPG POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink Plugin_EditorSaveObjectRPG.so ${CMAKE_BINARY_DIR}/bin/plugins/editor/saveObject/Plugin_EditorSaveObjectRPG.so)
ENDIF(WIN32)

install(TARGETS Plugin_EditorSaveObjectRPG
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/plugins/editor/saveObject
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
