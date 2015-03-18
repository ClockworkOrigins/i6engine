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
