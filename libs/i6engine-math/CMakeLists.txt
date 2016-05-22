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
# i6gengine-mathcmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

set(srcdir ./src)

set(i6mathSrc
	${srcdir}/i6eQuaternion.cpp
	${srcdir}/i6eVector.cpp
	${srcdir}/i6eVector2.cpp
	${srcdir}/i6eVector4.cpp
)

SOURCE_GROUP(math /src/*)

IF(ISIXE_BUILD_SHARED)
	add_library(i6engine-math SHARED ${i6mathSrc})
ELSE(ISIXE_BUILD_SHARED)
	add_library(i6engine-math STATIC ${i6mathSrc})
ENDIF(ISIXE_BUILD_SHARED)

target_link_libraries(i6engine-math i6engine-utils ${BOOST_LIBRARIES} ${BULLET_LIBRARIES} ${OGRE_LIBRARIES})
# to find the dependency libs during runtime
set_target_properties(i6engine-math PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

IF(WIN32 AND ISIXE_BUILD_SHARED)
	add_custom_command(TARGET i6engine-math POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6engine-math.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32 AND ISIXE_BUILD_SHARED)

########################
# i6engine-math tests
########################

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS i6engine-math
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
