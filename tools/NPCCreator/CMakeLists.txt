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
# NPCCreator cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
include_directories(${I6ENGINE_RPG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

set(srcdir ./src)
SET(includedir ./include)

set(NPCCreatorSrc
	${srcdir}/main.cpp
	
	${srcdir}/widgets/MainWindow.cpp
	${srcdir}/widgets/NPCEditWidget.cpp
	${srcdir}/widgets/NPCListWidget.cpp
	${srcdir}/widgets/NPCRenderWidget.cpp
)

QT5_WRAP_UI(NPCCreatorUI
	gui/wndMainWindow.ui
	gui/widgetNPCEdit.ui
	gui/widgetNPCList.ui
	gui/widgetNPCRender.ui
)

QT5_WRAP_CPP(NPCCreatorGuiHeader
	${includedir}/widgets/MainWindow.h
	${includedir}/widgets/NPCEditWidget.h
	${includedir}/widgets/NPCListWidget.h
	${includedir}/widgets/NPCRenderWidget.h
)

SOURCE_GROUP(moc /moc_*)

ADD_EXECUTABLE(NPCCreator ${GUI_TYPE} ${NPCCreatorSrc} ${NPCCreatorUI} ${NPCCreatorGuiHeader})

target_link_libraries(NPCCreator i6engine-rpg ${QT_LIBRARIES})

IF(WIN32 AND ISIXE_BUILD_SHARED)
	add_custom_command(TARGET NPCCreator POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/NPCCreator.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32 AND ISIXE_BUILD_SHARED)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS NPCCreator
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
