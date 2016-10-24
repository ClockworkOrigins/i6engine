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
# TakeControl cmake
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

set(TakeControlSrc
	${srcdir}/main.cpp
	
	${srcdir}/widgets/MainWindow.cpp
)

QT5_WRAP_CPP(TakeControlGuiHeader
	${includedir}/widgets/MainWindow.h
)

SET(ResourceFile resources/resources.qrc)

source_group(widgets /widgets/*)
SOURCE_GROUP(moc /moc_*)
SOURCE_GROUP(resources /resources/*)

# add resources ...
QT5_ADD_RESOURCES(QT_RESOURCE_FILES ${ResourceFile})

# add translations ...
QT5_ADD_TRANSLATION(QT_TRANSLATION_FILES ${TRANS})

IF(WIN32)
   set(WINDOWS_RC_FILES
         ${CMAKE_SOURCE_DIR}/config/icon.rc
         )
ENDIF(WIN32)

ADD_EXECUTABLE(i6eTakeControl ${GUI_TYPE} ${TakeControlSrc} ${TakeControlGuiHeader} ${QT_TRANSLATION_FILES} ${QT_RESOURCE_FILES} ${WINDOWS_RC_FILES})

target_link_libraries(i6eTakeControl toolsCommon ${CLOCKUTILS_LIBRARIES} ${QT_LIBRARIES})

IF(UNIX)
	target_link_libraries(i6eTakeControl pthread)
ENDIF(UNIX)

add_custom_command(TARGET i6eTakeControl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/de_DE.qm ${CMAKE_BINARY_DIR}/bin)
add_custom_command(TARGET i6eTakeControl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/en_US.qm ${CMAKE_BINARY_DIR}/bin)

IF(WIN32)
	add_custom_command(TARGET i6eTakeControl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6eTakeControl.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS i6eTakeControl
	DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
)
