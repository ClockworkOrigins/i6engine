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
