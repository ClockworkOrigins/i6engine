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
# i6Archiver cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

set(i6ArchiverSrc
	main.cpp
)

IF(WIN32)
   set(WINDOWS_RC_FILES
         ${CMAKE_SOURCE_DIR}/config/icon.rc
         )
ENDIF(WIN32)

add_executable(i6Archiver ${i6ArchiverSrc} ${WINDOWS_RC_FILES})
set_target_properties(i6Archiver PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(i6Archiver i6engine-utils ${BOOST_LIBRARIES} ${CLOCKUTILS_LIBRARIES} ${OGRE_LIBRARIES})

IF(WIN32)
	add_custom_command(TARGET i6Archiver POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6Archiver.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

IF(UNIX)
	target_link_libraries(i6Archiver pthread)
ENDIF(UNIX)

install(TARGETS i6Archiver
		DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
	)
