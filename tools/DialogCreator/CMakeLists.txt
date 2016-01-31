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
# DialogCreator cmake
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

set(dialogCreatorSrc
	${srcdir}/main.cpp
	
	${srcdir}/widgets/DialogHeaderWidget.cpp
	${srcdir}/widgets/DialogListWidget.cpp
	${srcdir}/widgets/MainWindow.cpp
	${srcdir}/widgets/NpcListWidget.cpp
	${srcdir}/widgets/ScriptTabWidget.cpp
)

QT5_WRAP_UI(dialogCreatorUI
	gui/widgetDialogHeader.ui
	gui/widgetDialogList.ui
	gui/widgetNpcList.ui
	gui/widgetScriptTab.ui
	gui/wndMainWindow.ui
)

QT5_WRAP_CPP(dialogCreatorGuiHeader
	${includedir}/widgets/DialogHeaderWidget.h
	${includedir}/widgets/DialogListWidget.h
	${includedir}/widgets/MainWindow.h
	${includedir}/widgets/NpcListWidget.h
	${includedir}/widgets/ScriptTabWidget.h
)

source_group(widgets /widgets/*)
SOURCE_GROUP(moc /moc_*)

ADD_EXECUTABLE(DialogCreator ${GUI_TYPE} ${dialogCreatorSrc} ${dialogCreatorUI} ${dialogCreatorGuiHeader})

target_link_libraries(DialogCreator i6engine-rpg ${QT_LIBRARIES})

IF(WIN32)
	add_custom_command(TARGET DialogCreator POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/DialogCreator.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

configure_file(${CMAKE_SOURCE_DIR}/config/DialogCreator.ini ${CMAKE_BINARY_DIR}/bin/DialogCreator.ini COPYONLY)

install(FILES ${CMAKE_SOURCE_DIR}/config/DialogCreator.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/config)

install(TARGETS DialogCreator
	DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
)
