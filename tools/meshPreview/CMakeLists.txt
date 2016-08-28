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
# meshPreview cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

set(MeshPreviewSrc
	main.cpp

	MainWindow.cpp
)

QT5_WRAP_CPP(MeshPreviewHeader
	MainWindow.h
)

IF(WIN32)
   set(WINDOWS_RC_FILES
         ${CMAKE_SOURCE_DIR}/config/icon.rc
         )
ENDIF(WIN32)

add_executable(i6eMeshPreview ${GUI_TYPE} ${MeshPreviewSrc} ${MeshPreviewHeader} ${WINDOWS_RC_FILES})
set_target_properties(i6eMeshPreview PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(i6eMeshPreview i6engine-modules ${OGRE_LIBRARIES} ${QT_LIBRARIES})

IF(NOT WIN32)
	target_link_libraries(i6eMeshPreview pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET i6eMeshPreview POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6eMeshPreview.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS i6eMeshPreview
		DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
	)