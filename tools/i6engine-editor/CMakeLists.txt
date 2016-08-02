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
# i6engine-editor cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

set(srcdir ./src)
SET(includedir ./include/i6engine/editor)

set(editorSrc
	${srcdir}/main.cpp

	${srcdir}/Editor.cpp
	
	${srcdir}/widgets/ConfigDialog.cpp
	${srcdir}/widgets/MainWindow.cpp
	${srcdir}/widgets/ObjectContainerWidget.cpp
	${srcdir}/widgets/ObjectInfoWidget.cpp
	${srcdir}/widgets/ObjectListWidget.cpp
	${srcdir}/widgets/RenderWidget.cpp
	${srcdir}/widgets/TemplateListWidget.cpp
)

QT5_WRAP_UI(editorUI
	gui/dialogConfig.ui
	gui/widgetObjectContainer.ui
	gui/widgetObjectInfo.ui
	gui/widgetObjectList.ui
	gui/widgetRender.ui
	gui/widgetTemplateList.ui
	gui/wndMainWindow.ui
)

QT5_WRAP_CPP(editorGuiHeader
	${includedir}/widgets/ConfigDialog.h
	${includedir}/widgets/MainWindow.h
	${includedir}/widgets/ObjectContainerWidget.h
	${includedir}/widgets/ObjectInfoWidget.h
	${includedir}/widgets/ObjectListWidget.h
	${includedir}/widgets/RenderWidget.h
	${includedir}/widgets/TemplateListWidget.h
)

SET(ResourceFile resources/resources.qrc)

source_group(components /components/*)
source_group(gui /gui/*)
source_group(misc /misc/*)
source_group(plugins /plugins/*)
source_group(widgets /widgets/*)
SOURCE_GROUP(moc /moc_*)
SOURCE_GROUP(resources /resources/*)
SOURCE_GROUP(resources /qrc_*)

# add resources ...
QT5_ADD_RESOURCES(QT_RESOURCE_FILES ${ResourceFile})

# add translations ...
QT5_ADD_TRANSLATION(QT_TRANSLATION_FILES ${TRANS})

IF(WIN32)
   set(WINDOWS_RC_FILES
         ${CMAKE_SOURCE_DIR}/config/icon.rc
         )
ENDIF(WIN32)

ADD_EXECUTABLE(i6engine-editor ${GUI_TYPE} ../common/src/Config.cpp ${editorSrc} ${editorUI} ${editorGuiHeader} ${QT_TRANSLATION_FILES} ${QT_RESOURCE_FILES} ${WINDOWS_RC_FILES})

target_link_libraries(i6engine-editor i6engine-modules Plugin_EditorTypeAngle Plugin_EditorTypeBool Plugin_EditorTypeColour Plugin_EditorTypeDouble Plugin_EditorTypeInteger Plugin_EditorTypeLightType Plugin_EditorTypeMoverInterpolateMode Plugin_EditorTypeMoverInterpolateWay Plugin_EditorTypeMoverPositioning Plugin_EditorTypeQuaternion Plugin_EditorTypeShapeType Plugin_EditorTypeShatterInterest Plugin_EditorTypeString Plugin_EditorTypeVec3 Plugin_EditorTypeVec4 ${QT_LIBRARIES})

IF(UNIX)
	target_link_libraries(i6engine-editor pthread)
ENDIF(UNIX)

add_custom_command(TARGET i6engine-editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/de_DE.qm ${CMAKE_BINARY_DIR}/bin)
add_custom_command(TARGET i6engine-editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/en_US.qm ${CMAKE_BINARY_DIR}/bin)

IF(WIN32)
	add_custom_command(TARGET i6engine-editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6engine-editor.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS i6engine-editor
	DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
)

install(FILES ${CMAKE_SOURCE_DIR}/config/i6engine-editor.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/config)

configure_file(${CMAKE_SOURCE_DIR}/config/i6engine-editor.ini ${CMAKE_BINARY_DIR}/bin/i6engine-editor.ini COPYONLY)
