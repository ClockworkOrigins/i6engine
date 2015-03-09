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
# i6engine-editor cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

set(srcdir ./src)

set(editorSrc
	${srcdir}/Editor.cpp
	
	${srcdir}/gui/FileDialogWidget.cpp
	${srcdir}/gui/ListboxWidget.cpp
	${srcdir}/gui/MenuBarWidget.cpp
	${srcdir}/gui/ObjectInfoWidget.cpp
)

source_group(components /components/*)
source_group(gui /gui/*)
source_group(misc /misc/*)

IF(ISIXE_BUILD_SHARED)
	add_library(i6engine-editor SHARED ${editorSrc})
ELSE(ISIXE_BUILD_SHARED)
	add_library(i6engine-editor STATIC ${editorSrc})
ENDIF(ISIXE_BUILD_SHARED)

target_link_libraries(i6engine-editor i6engine-modules)

IF(WIN32 AND ISIXE_BUILD_SHARED)
	add_custom_command(TARGET i6engine-editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6engine-editor.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32 AND ISIXE_BUILD_SHARED)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS i6engine-editor
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
