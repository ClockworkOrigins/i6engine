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
# i6gengine-utils cmake
################################

include_directories("include")

set(srcdir ./src)

set(i6utilsSrc
	${srcdir}/Exceptions.cpp
	${srcdir}/ExceptionQueue.cpp
	${srcdir}/i6eRandom.cpp
	${srcdir}/Profiling.cpp
	${srcdir}/RealTimeClock.cpp
	${srcdir}/Semaphore.cpp
	${srcdir}/Splash.cpp
)

IF(ISIXE_WITH_LOGGING)
	set(i6utilsSrc
		${i6utilsSrc}
		${srcdir}/Logger.cpp
	)
ENDIF(ISIXE_WITH_LOGGING)

SOURCE_GROUP(utils src/*)

IF(ISIXE_BUILD_SHARED)
	add_library(i6engine-utils SHARED ${i6utilsSrc})
ELSE(ISIXE_BUILD_SHARED)
	add_library(i6engine-utils STATIC ${i6utilsSrc})
ENDIF(ISIXE_BUILD_SHARED)

target_link_libraries(i6engine-utils ${BOOST_LIBRARIES} ${BULLET_LIBRARIES})
# to find the dependency libs during runtime
set_target_properties(i6engine-utils PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

IF(NOT WIN32)
	target_link_libraries(i6engine-utils X11 jpeg ${PYTHON_LIBRARIES})
ENDIF(NOT WIN32)

IF(WIN32 AND ISIXE_BUILD_SHARED)
	add_custom_command(TARGET i6engine-utils POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6engine-utils.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32 AND ISIXE_BUILD_SHARED)
########################
# i6engine-utils tests
########################

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS i6engine-utils
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
