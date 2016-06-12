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
# i6Archive plugin cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_EDITOR_INCLUDE_DIR})

set(srcdir ./src)

set(plugini6ArchiveSrc
	${srcdir}/i6Archive.cpp
	${srcdir}/i6ArchiveFactory.cpp
	${srcdir}/i6ArchivePlugin.cpp
)

add_library(Plugin_i6Archive SHARED ${plugini6ArchiveSrc})

target_link_libraries(Plugin_i6Archive i6engine-utils ${BOOST_LIBRARIES} ${CLOCKUTILS_LIBRARIES} ${OGRE_LIBRARIES})

IF(UNIX)
	set_target_properties(Plugin_i6Archive PROPERTIES PREFIX "")
	set_target_properties(Plugin_i6Archive PROPERTIES SUFFIX ".so.1.9.0")
ENDIF(UNIX)

IF(WIN32)
	add_custom_command(TARGET Plugin_i6Archive POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/Plugin_i6Archive.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS Plugin_i6Archive
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
