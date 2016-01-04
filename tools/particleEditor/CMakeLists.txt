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
# ParticleEditor cmake
################################

INCLUDE_DIRECTORIES("include")
INCLUDE_DIRECTORIES(${I6ENGINE_UTILS_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${I6ENGINE_MATH_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${I6ENGINE_CORE_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${I6ENGINE_MODULES_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/plugins/particleUniverse/include)

set(srcdir ./src)
SET(includedir ./include)

set(particleEditorSrc
	${srcdir}/main.cpp
	
	${srcdir}/widgets/MainWindow.cpp
	${srcdir}/widgets/WidgetParticleList.cpp
	${srcdir}/widgets/WidgetRender.cpp
	${srcdir}/widgets/WidgetScript.cpp
)

QT5_WRAP_UI(particleEditorUI
	gui/widgetParticleList.ui
	gui/widgetRender.ui
	gui/widgetScript.ui
	gui/wndMainWindow.ui
)

QT5_WRAP_CPP(particleEditorGuiHeader
	${includedir}/widgets/MainWindow.h
	${includedir}/widgets/WidgetParticleList.h
	${includedir}/widgets/WidgetRender.h
	${includedir}/widgets/WidgetScript.h
)

source_group(widgets /widgets/*)
SOURCE_GROUP(moc /moc_*)

ADD_EXECUTABLE(ParticleEditor ${GUI_TYPE} ${particleEditorSrc} ${particleEditorUI} ${particleEditorGuiHeader})

target_link_libraries(ParticleEditor i6engine-modules ${QT_LIBRARIES})

IF(WIN32)
	add_custom_command(TARGET ParticleEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/ParticleEditor.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS ParticleEditor
	DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
)
